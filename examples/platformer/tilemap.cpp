#include "tilemap.h"

#include <ecs/Transform.h>
#include <renderer/rData.h>
#include <renderer/Renderer.h>

namespace Platformer
{
    TileMap::TileMap(float _tile_width, float _tile_height, float _z_index) {
        this->tile_width = _tile_width;
        this->tile_height = _tile_height;

        this->z_index = _z_index;

        this->errMsgPrinted = false;

        this->tiles.clear();
    }

    void TileMap::setTile(glm::ivec2 pos, uint32_t val) {
        for (Tile &t : this->tiles) {
            if (t.pos == pos) {
                t.index = val;
                return;
            }
        }

        this->tiles.push_back(Tile {pos, val});
    }

    void TileMap::removeTile(glm::ivec2 pos) {
        for (uint32_t i = 0; i < this->tiles.size(); i++) {
            if (this->tiles[i].pos == pos) {
                this->tiles.erase(this->tiles.begin() + i);
                break;
            }
        }
    }

    int TileMap::update(double) {
        return this->toRData(Pontilus::Renderer::quadPool);
    }

    void TileMap::collide(const Pontilus::ECS::Transform &t, std::vector<TileMap::Tile> &out) {
        using namespace Pontilus::ECS;
        out = std::vector<TileMap::Tile>();

        // we find all possible tiles the object could collide with
        // start with bounds of Transform
        glm::vec3 max3 = t.pos + t.whd / 2.0f;
        glm::vec3 min3 = t.pos - t.whd / 2.0f;
        // for sake of better floor detection, extend min down a bit
        min3.y -= 0.1f;

        // find outermost tile bounds, after transforming to tile space. we extend a little bit
        // so that we only check who's centers are within the max and min
        glm::vec3 tileWhd = glm::vec3(this->tile_width, this->tile_height, 1.0f);

        max3 = glm::ceil((max3 + (tileWhd / 2.0f)) / tileWhd);
        min3 = glm::floor((min3 - (tileWhd / 2.0f)) / tileWhd);

        glm::vec2 max = glm::vec2(max3) - (1.0f / 2.0f);
        glm::vec2 min = glm::vec2(min3) + (1.0f / 2.0f);

        // with our max and min, let's loop over our tiles to see which one's are actually colliding
        for (Tile tile : this->tiles) {
            if (!(min.x <= tile.pos.x && tile.pos.x <= max.x)) {
                continue;
            }
            if (!(min.y <= tile.pos.y && tile.pos.y <= max.y)) {
                continue;
            }

            out.push_back(tile);
        }
    }

    int TileMap::toRData(Pontilus::Renderer::rData &r) {
        using namespace Pontilus::ECS;
        using namespace Pontilus::Renderer;

        Transform *tptr = (Transform *)this->parent->getComponent(typeid(Transform));
        Transform t;
        if (!tptr) {
            // it's safe to just create a transform at the point 0, 0
            if (!this->errMsgPrinted) {
                __pMessage("TileMap doesn't have a Transform, so creating one at origin.");
                this->errMsgPrinted = true;
            }
            t = Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
        } else {
            t = Transform(*tptr);
        }

        //__pAssert(!(rOffset >= r.vertCount / 4), "rData not big enough to hold game states!");

        int stride = r.dataOffset;

        for (auto tile : this->tiles) {

            glm::vec3 coords = glm::vec3(((float) tile.pos.x) * this->tile_width, ((float) tile.pos.y) * this->tile_height, this->z_index) + t.pos;

            // now that we got the location, let's create the corners of this tile
            glm::vec3 to_corner = glm::vec3(this->tile_width, this->tile_height, 0.0f) / 2.0f;
            glm::vec3 corners[4] = {
                coords + to_corner,
                coords + to_corner * glm::vec3(-1.0, 1.0, 1.0),
                coords + to_corner * -1.0f,
                coords + to_corner * glm::vec3(1.0, -1.0, 1.0)
            };

            for (int i = 0; i < 4; i++)
                {
                    off_len result = getAttribMetaData(r, PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        memcpy((char *) r.data + result.first + stride, value_ptr(corners[i]), 3 * sizeof(float));
                        //for (int j = 0; j < 3; j++)
                        //{
                        //    ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&t.pos)[j];
                        //}
                    }
                    
                    result = getAttribMetaData(r, PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = 1.0f;
                        }             
                    }

                    result = getAttribMetaData(r, PONT_TEXCOORD);
                    
                    if (result.second >= 2 * sizeof(float))
                    {
                        //orientation.x /= t.whd.x;
                        //orientation.y /= t.whd.y;
                        for (int j = 0; j < 2; j++)
                        {
                            //((float *)((char *)r.data + result.first + stride))[j] = this->tex.source == nullptr ? 0.0 : this->tex.texCoords[j + i * 2];
                            ((float *)((char *)r.data + result.first + stride))[j] = 0.0f;
                        }
                    }

                    result = getAttribMetaData(r, PONT_TEXID);
                    if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                    {
                        /*
                        if (this->tex.source == nullptr)
                        {
                            *(float *)((char *)r.data + result.first + stride) = 0.0f;
                        }
                        else
                        {
                            *(float *)((char *)r.data + result.first + stride) = this->tex.source == nullptr ? 0.0 : this->tex.source->texID;
                        }
                        */
                       *(float *)((char *)r.data + result.first + stride) = 0.0f;
                    }
                    stride += getLayoutLen(r);
                }

                r.isDirty = true;
                
                r.dataOffset = stride;
        }
        
        return 0;
    }
}