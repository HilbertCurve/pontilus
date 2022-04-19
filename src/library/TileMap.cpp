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
        void getTileMap(unsigned n, unsigned k, int *key, TileMap &t, float tilewidth, Pontilus::Graphics::IconMap *tileset) {
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
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < k; j++) {
                    int tiletex = t.key[i + j * n];
                    if (tiletex >= 0) {
                        Tile tile = Tile();
                        tile.init({i * tilewidth, (k - (j + 1)) * tilewidth, 0.0f}, t.color, tilewidth, tilewidth);

                        tile.tile_coords = {i, (k - (j + 1))};

                        SpriteRenderer s = SpriteRenderer();
                        if (tileset) {
                            s.init(Pontilus::Graphics::getTexture(*tileset, tiletex));
                        } else {
                            s.init({nullptr});
                        }

                        t.tiles.push_back({tile, s});
                    }
                }
            }

            for (int i = 0; i < t.size(); i++) {
                t.at(i).addComponent(t.tiles.at(i).second);
                Pontilus::getCurrentScene()->objs.push_back(&t.at(i));
            }
        }

        void deleteTileMap(TileMap &t) {
            for (int i = 0; i < t.size(); i++) {
                t.at(i) = Tile();
                Pontilus::getCurrentScene()->removeObj(t[i].id);
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

            for (int i = 0; i < t.size(); i++) {
                if (t[i].tile_coords == coords) {
                    return;
                }
            }

            Tile _tile;
            _tile.init({coords.x * t.tilewidth, (coords.y) * t.tilewidth, 0.0f}, t.color, t.tilewidth, t.tilewidth);

            _tile.tile_coords = coords;
            Engine::ECS::SpriteRenderer _renderer = Engine::ECS::SpriteRenderer();
            _renderer.init(Graphics::getTexture(*t.tileset, i_tile));

            t.tiles.push_back({_tile, _renderer});

            t.at(t.size() - 1).addComponent(t.tiles.at(t.size() - 1).second);

            Pontilus::getCurrentScene()->objs.push_back(&t.at(t.size() - 1));
            // TODO: insert value tile into t.key

            t.key[coords.x + t.width * (t.height - (coords.y + 1))] = i_tile;
        }

        void removeTile(TileMap &t, glm::vec<2, int> coords) {
            if (!Math::between({-1, -1}, coords, {t.width + 1, t.height + 1})) {
                return;
            }

            for (int i = 0; i < t.size(); i++) {
                if (t[i].tile_coords == coords) {
                    Tile &tile = t[i];
                    auto scene = Pontilus::getCurrentScene();
                    for (int j = 0; j < scene->objs.size(); j++) {
                        if (scene->objs[j]->id == tile.id) {
                            t.at(i) = Tile();
                            scene->removeObj(tile.id);
                            t.at(i).tile_coords = {-1, -1};
                            t.key[coords.x + t.width * (t.height - (coords.y + 1))] = -1;
                            return;
                        }
                    }
                }
            }
        }

        void applyColorFilter(TileMap &t, glm::vec4 color) {
            for (tile_renderer &tile : t.tiles) {
                tile.first.color = color;
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
                if (Math::between(p_tileCoordsMin, tilemap[i].pos, p_tileCoordsMax)) surrounding.push_back(tilemap[i]);
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

    }
}