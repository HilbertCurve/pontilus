#include "tilemap.h"

#include <ecs/Transform.h>
#include <renderer/rData.h>
#include <renderer/Renderer.h>

namespace Platformer
{
    using namespace Pontilus::Renderer;
    TileMap::TileMap(float _tile_width, float _tile_height, float _z_index) {
        this->tile_width = _tile_width;
        this->tile_height = _tile_height;

        this->z_index = _z_index;

        // error message is used for toRData, to let user know there isn't a transform
        // attached to the tilemap's parent
        this->errMsgPrinted = false;

        this->tiles.clear();
        this->im = nullptr;
    }

    TileMap::~TileMap() {
        if (this->im)
            delete this->im;
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

    void TileMap::setTextures(std::string source, uint32_t width, uint32_t height, uint32_t padding) {
        if (this->im) {
            delete this->im;
        }

        this->im = new IconMap(source.c_str(), width, height, padding);
    }

    int TileMap::update(double) {
        auto quadTarget = RendererController::get().getTarget(1);

        return this->toRData(*std::get<0>(quadTarget));
    }

    void TileMap::collide(const Pontilus::ECS::Transform &t, std::vector<TileMap::Tile> &out) {
        using namespace Pontilus::ECS;
        out = std::vector<TileMap::Tile>();

        // we find all possible tiles the object could collide with
        // start with bounds of Transform
        glm::vec3 max3 = t.pos + t.whd / 2.0f;
        glm::vec3 min3 = t.pos - t.whd / 2.0f;

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

    int TileMap::toRData(rData &r) {
        using namespace Pontilus::ECS;

        Transform *tptr = (Transform *)this->parent->getComponent(typeid(Transform));
        Transform t;
        if (!tptr) {
            // it's safe to just create a transform at the point 0, 0
            if (!this->errMsgPrinted) {
                _pMessage("TileMap doesn't have a Transform, so creating one at origin.");
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

            Texture texture = this->im ? this->im->get(tile.index) : IconMap::emptyTexture();
            for (int i = 0; i < 4; i++)
                {
                    auto result = r.getAttribMetaData(PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        memcpy((char *) r.data + result.first + stride, value_ptr(corners[i]), 3 * sizeof(float));
                        //for (int j = 0; j < 3; j++)
                        //{
                        //    ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&t.pos)[j];
                        //}
                    }
                    
                    result = r.getAttribMetaData(PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = 1.0f;
                        }             
                    }

                    result = r.getAttribMetaData(PONT_TEXCOORD);
                    if (result.second >= 2 * sizeof(float))
                    {
                        //orientation.x /= t.whd.x;
                        //orientation.y /= t.whd.y;
                        for (int j = 0; j < 2; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = texture.source == nullptr ? 0.0 : texture.texCoords[j + i * 2];
                            //((float *)((char *)r.data + result.first + stride))[j] = 0.0f;
                        }
                    }

                    result = r.getAttribMetaData(PONT_TEXID);
                    if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                    {
                        *(float *)((char *)r.data + result.first + stride) = texture.source == nullptr ? 0.0 : texture.source->id();
                    }

                    stride += r.getLayoutLen();
                }

                r.isDirty = true;
                
                r.dataOffset = stride;
        }
        
        return 0;
    }
}