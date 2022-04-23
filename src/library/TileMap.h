#pragma once

#include <vector>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <graphics/Texture.h>
#include <utils/Utils.h>

// This tilemap system allows for loading arrays of objects, 
// specifying their textures using an IconMap, and detecting 
// collision between an object and its surroundings. 
// It does not resolve collisions.

namespace Pontilus {
    namespace Library {
        class Tile : public Engine::ECS::GameObject {
            public:
            bool collides = true;
            glm::vec<2, int> tile_coords;
        };

        // TODO: refactor to AABB
        class rect {
            public:
            glm::vec2 min, max;
        };

        typedef Pair<Tile, rect> tile_rect;
        typedef Pair<Tile, Pontilus::Engine::ECS::SpriteRenderer> tile_renderer;
        // Collision info datasets are arrays of colliding tiles and the
        // rectangle of corresponding tiles over a certain game object
        typedef std::vector<tile_rect> CollisionInfo;

        class TileMap {
            public:
            // TODO: a public .init() function
            std::vector<tile_renderer> tiles;
            int *key;
            float tilewidth;
            int width, height;
            glm::vec4 color;
            Graphics::IconMap *tileset;
            size_t size() {
                return tiles.size();
            }
            Tile &operator[](int i) {
                return tiles[i].first;
            }
            Tile &at(int i) {
                return tiles.at(i).first;
            }

        };

        // automatically adds objects to current scene
        void getTileMap(unsigned n, unsigned k, int *key, TileMap &t, float tilewidth, Pontilus::Graphics::IconMap *tileset);
        void deleteTileMap(TileMap &t);

        rect rectFromObj(Engine::ECS::GameObject o);
        void addTile(TileMap &t, glm::vec<2, int> coords, float tile);
        void removeTile(TileMap &t, glm::vec<2, int> coords);
        void applyColorFilter(TileMap &t, glm::vec4 color);
        //void applyCollisionFilter(int *tiles, int count);

        ////////////////////////
        // Collision detection
        ////////////////////////

        bool detectRectRect(rect a, rect b);
        void getCollisionInfo(Engine::ECS::GameObject &obj, CollisionInfo &info, TileMap &tilemap);

        ///////////////////////////
        // Serialization and Deserialization
        ///////////////////////////
        void serializeTileMap(const char *filepath, TileMap &t);
        void deserializeTileMap(const char *filepath, TileMap &t);
    }
}

