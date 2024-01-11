#pragma once

#include <glm/glm.hpp>

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
    class TileMap
    {
        private:
        struct Tile {
            glm::vec3 pos;
            uint32_t value;
        };
    };
}