#pragma once

#include <ecs/Component.h>
#include <ecs/Transform.h>
#include <renderer/rData.h>
#include <renderer/Texture.h>
#include <glm/glm.hpp>
#include <map>

namespace Platformer
{
    /**
     * You may be asking: what is a TileMap?
     * Internally, it's a map from RxR -> Tile, where Tile is the set of possible tiles
     * a TileMap can have.
     * Externally, it's a bunch of rectangles rendered to the screen. You can check if a certain shape
     * is colliding with the map and act accordingly.
     * Unlike with the Player, it makes sense that you can have multiple TileMaps. One for background, one
     * for foreground, etc.
     */
    class TileMap : public Pontilus::ECS::Component, public Pontilus::Renderer::Renderable
    {
        public:
        struct Tile {
            glm::ivec2 pos;
            uint32_t index;
        };

        TileMap() = default;
        TileMap(float tile_width, float tile_height, float z_index);

        /**
         * This function overwrites existing tiles and creates new ones if it doesn't exist.
         */
        void setTile(glm::ivec2 pos, uint32_t val);
        /**
         * Whenever this function returns a non-integer value, a debug message will
         * be printed, likely because the tile at `pos` doesn't exist.
         */
        void removeTile(glm::ivec2 pos);

        virtual int update(double);

        void collide(const Pontilus::ECS::Transform &t, std::vector<Tile> &out);

        // getter
        float width() { return tile_width; }
        float height() { return tile_height; }

        private:
        std::vector<Tile> tiles;
        float z_index;
        float tile_width, tile_height;
        bool errMsgPrinted;
        Pontilus::Renderer::IconMap *im;
        virtual int toRData(Pontilus::Renderer::rData &r);
    };
}