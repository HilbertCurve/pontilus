#include "TileMap.h"

#include <vector>
#include <core/Application.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <graphics/Texture.h>
#include <utils/PMath.h>
#include <utils/Utils.h>

namespace Pontilus {
    namespace Library {
        rect rectFromObj(Pontilus::Engine::ECS::GameObject obj) {
            return {
                {obj.pos.x - obj.width / 2.0f, obj.pos.y - obj.height / 2.0f},
                {obj.pos.x + obj.width / 2.0f, obj.pos.y + obj.height / 2.0f},
            };
        }

        // automatically adds objects to current scene
        void getTileMap(unsigned n, unsigned k, int *key, TileMap &t, float tilewidth, Pontilus::Renderer::IconMap *tileset) {
            // empty tiles
            t.tiles.clear();
            t.tiles.reserve(n*k);
            t.tilewidth = tilewidth;
            t.width = n;
            t.height = k;
            t.key = key;
            t.color = {1.0f, 1.0f, 1.0f, 1.0f};
            t.tileset = tileset;
            using namespace Pontilus::Engine::ECS;
            // loop through key, inserting tiles if key[i + j*n] >= 0
            for (unsigned i = 0; i < n; i++) {
                for (unsigned j = 0; j < k; j++) {
                    int tiletex = t.key[i + j * n];
                    Tile tile;
                    tile.init({i * tilewidth, (k - (j + 1)) * tilewidth, 0.0f}, tilewidth, tilewidth);
                    SpriteRenderer s = SpriteRenderer();
                    tile.tile_coords = {i, (k - (j + 1))};
                    if (tiletex >= 0) {
                        tile.active = true;
                        tile.collides = true;

                        if (tileset) {
                            s.init(Pontilus::Renderer::getTexture(*tileset, tiletex), t.color);
                        } else {
                            s.init({nullptr}, t.color);
                        }

                    } else {
                        s.init({nullptr}, {0.0f, 0.0f, 0.0f, 0.0f});
                    }

                    t.tiles.push_back({tile, s});

                    if (tiletex >= 0) {
                        t.at(t.size() - 1).addComponent(t.tiles.at(t.size() - 1).second);
                        Pontilus::getCurrentScene()->addObject(&t.at(t.size() - 1));
                    }
                }
            }
        }

        void deleteTileMap(TileMap &t) {
            for (int i = 0; i < t.size(); i++) {
                bool alreadyDeleted = t.at(i).currentScene == nullptr;
                t.at(i) = Tile();
                if (!alreadyDeleted)
                    Pontilus::getCurrentScene()->removeObject(t[i].id);
            }
            t.tiles.clear();
        }

        static glm::vec2 tileCoords(float x, float y, float tilewidth) {
            return {floor((x + tilewidth / 2) / tilewidth), floor((y + tilewidth / 2) / tilewidth)};
        }

        void addTile(TileMap &t, glm::vec<2, int> coords, float tile) {
            int i_tile = floor(tile);
            if (!Math::between({-1, -1}, coords, {t.width, t.height})) {
                return;
            }

            int posInArray = (t.height - (coords.y + 1)) + t.height * coords.x;
            if (t[posInArray].active) {
                return;
            }

            Tile &_tile = t.at(posInArray);
            _tile.active = true;
            _tile.collides = true;

            Engine::ECS::SpriteRenderer &_renderer = t.tiles.at(posInArray).second;
            _renderer = Engine::ECS::SpriteRenderer();
            if (t.tileset)
                _renderer.init(Renderer::getTexture(*t.tileset, i_tile), t.color);
            else
                _renderer.init({nullptr}, t.color);

            _tile.addComponent(_renderer);

            Pontilus::getCurrentScene()->addObject(&t.at(posInArray));

            t.key[coords.x + t.width * (t.height - (coords.y + 1))] = i_tile;
        }

        void removeTile(TileMap &t, glm::vec<2, int> coords) {
            if (!Math::between({-1, -1}, coords, {t.width, t.height})) {
                return;
            }

            for (unsigned i = 0; i < t.size(); i++) {
                if (t[i].tile_coords == coords) {
                    Tile &tile = t[i];
                    auto scene = Pontilus::getCurrentScene();
                    if (tile.currentScene == scene) {
                        scene->removeObject(tile.id);
                        t.key[coords.x + t.width * (t.height - (coords.y + 1))] = -1;
                        t.at(i).removeComponent(typeid(Engine::ECS::SpriteRenderer));

                        t.at(i).active = false;
                        t.at(i).collides = false;
                        t.tiles.at(i).second = Engine::ECS::SpriteRenderer();
                        return;
                    }
                }
            }
        }

        void applyColorFilter(TileMap &t, glm::vec4 color) {
            using namespace Engine::ECS;
            for (tile_renderer &tile : t.tiles) {
                tile.second.color = color;
            }
            t.color = color;
        }


        bool detectRectRect(rect a, rect b) {
            glm::vec2 awh = (a.max - a.min);
            rect r_t = {b.min - awh / 2.0f, b.max + awh / 2.0f};
            glm::vec2 a_c = (a.max + a.min) / 2.0f;
            
            return Math::between(r_t.min.x, a_c.x, r_t.max.x) &&
                Math::between(r_t.min.y, a_c.y, r_t.max.t);
        }

        void getCollisionInfo(Pontilus::Engine::ECS::GameObject &obj, CollisionInfo &info, TileMap &tilemap) {
            info.clear();

            glm::vec2 obj_v[4] = {
                glm::vec2(obj.pos) + glm::vec2{-obj.width,  obj.height} / 2.0f, 
                glm::vec2(obj.pos) + glm::vec2{ obj.width,  obj.height} / 2.0f,
                glm::vec2(obj.pos) + glm::vec2{ obj.width, -obj.height} / 2.0f, 
                glm::vec2(obj.pos) + glm::vec2{-obj.width, -obj.height} / 2.0f,
            };

            std::vector<Tile> surrounding;
            surrounding.clear();
            glm::vec2 p_tileCoordsMin, p_tileCoordsMax;
            p_tileCoordsMin = floor((obj_v[3] - tilemap.tilewidth / 2) / tilemap.tilewidth) * tilemap.tilewidth;
            p_tileCoordsMax = ceil((obj_v[1] + tilemap.tilewidth / 2) / tilemap.tilewidth) * tilemap.tilewidth;
            
            for (int i = 0; i < tilemap.size(); i++) {
                if (Math::between(p_tileCoordsMin, tilemap[i].pos, p_tileCoordsMax) && tilemap[i].collides) surrounding.push_back(tilemap[i]);
            }

            for (int i = 0; i < surrounding.size(); i++) {
                Tile t = surrounding[i];
                rect r_obj = {obj_v[3], obj_v[1]};
                rect r_t = {glm::vec2(t.pos) - glm::vec2{t.width, t.height} / 2.0f, glm::vec2(t.pos) + glm::vec2{t.width, t.height} / 2.0f};
                if (!detectRectRect(r_obj, r_t)) continue;
                
                glm::vec2 min = {fmax(r_obj.min.x, r_t.min.x), fmax(r_obj.min.y, r_t.min.y)};
                glm::vec2 max = {fmin(r_obj.max.x, r_t.max.x), fmin(r_obj.max.y, r_t.max.y)};

                info.push_back({t, {min, max}});
            }
        }


        DirFlag collide(Engine::ECS::GameObject &obj, bool emphasizeFloor, TileMap &tilemap) {
    // collision
    std::vector<Library::tile_rect> info;
    if (emphasizeFloor)
        obj.height += 0.01;
    Library::getCollisionInfo(obj, info, tilemap);
    if (emphasizeFloor)
        obj.height -= 0.01;
    bool hasFloor = false;
    bool hasLeft = false, hasRight = false, hasUp = false;
    for (auto i : info) {
        float w = i.second.max.x - i.second.min.x;
        float h = i.second.max.y - i.second.min.y;
        
        // if it's a small corner overlap between tile on a wall and obj, obj 
        // could get caught on the top of one tile of the wall.
        if (w * h < __pEPSILON * 50 /* adjust */) continue;

        if (w / obj.width < h / obj.height) {
            if (i.first.pos.x > obj.pos.x) {
                if (obj.velocity.x >= 0.0f)
                    obj.velocity.x = 0.0f;
                if (!hasRight) {
                    obj.pos.x -= w;
                    hasRight = true;
                }
            } else {
                if (obj.velocity.x <= 0.0f)
                    obj.velocity.x = 0.0f;
                if (!hasLeft) { 
                    obj.pos.x += w;
                    hasLeft = true;
                }
            }
        } else {
            if (i.first.pos.y > obj.pos.y) {
                if (!hasUp) {
                    obj.velocity.y = 0.0f;
                    obj.pos.y -= h;
                    hasUp = true;
                }
            } else if (!hasFloor) {
                obj.velocity.y = 0.0f;
                obj.pos.y += h;
                if (emphasizeFloor)
                    obj.pos.y -= 0.01;
                hasFloor = true;
            }
        }
    }

    return ((hasFloor << 3) | (hasLeft << 2) | (hasUp << 1) | (hasRight << 0)) & 0b1111;
}
        
        ///////////////////////////
        // Serialization and Deserialization
        ///////////////////////////

        void serializeTileMap(const char *filepath, TileMap &t) {
            FILE *fp;
            fp = fopen(filepath, "w");
            if (!fp) {
                __pWarning("Opening file %s failed.", filepath);
                return;
            }

            fputc(t.width, fp);
            fputc(t.height, fp);
            fputc('\n', fp);

            for (int i = 0; i < t.width * t.height; i++) {
                fputc(t.key[i], fp);
            }

            fclose(fp);
        }

        void deserializeTileMap(const char *filepath, TileMap &t) {
            FILE *fp;
            fp = fopen(filepath, "r");
            if (!fp) {
                __pWarning("Opening file %s failed.", filepath);
                return;
            }

            int width = fgetc(fp);
            int height = fgetc(fp);
            fseek(fp, 1, SEEK_CUR);
            int m_width = fmin(width, t.width);
            int m_height = fmin(height, t.height);

            memset(t.key, 0, t.width * t.height);

            for (int i = 0; i < t.width * t.height; i++)
            {
                t.key[i] = (signed char) fgetc(fp);
            }

            Library::deleteTileMap(t);
            Library::getTileMap(t.width, t.height, t.key, t, t.tilewidth, t.tileset);
        }

    }
}

