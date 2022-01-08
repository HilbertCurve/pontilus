#include "ecs/SpriteRenderer.h"
#include "graphics/rData.h"
#include <typeinfo>

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            SpriteRenderer::SpriteRenderer()
            {
                this->tex = {nullptr};
            }

            using namespace Graphics; // fight me
            int SpriteRenderer::toRData(rData &r, unsigned int rOffset)
            {
                __pAssert(!(rOffset >= r.vertCount / 4), "rData not big enough to hold game states!");

                int stride = rOffset * getLayoutLen(r) * 4;
                for (int i = 0; i < 4; i++)
                {
                    glm::vec3 orientation;
                    switch (i)
                    {
                        case 0: orientation = {1.0f * parent->width, 1.0f * parent->height, 0.0f}; break;
                        case 1: orientation = {0.0f * parent->width, 1.0f * parent->height, 0.0f}; break;
                        case 2: orientation = {0.0f * parent->width, 0.0f * parent->height, 0.0f}; break;
                        case 3: orientation = {1.0f * parent->width, 0.0f * parent->height, 0.0f}; break;
                    }

                    off_len result = getAttribMetaData(r, PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        parent->pos += orientation - glm::vec3{parent->width / 2, parent->height / 2, 0.0f};

                        // TODO: just use memcpy, bonehead.
                        for (int j = 0; j < 3; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&parent->pos)[j];
                        }

                        parent->pos -= orientation - glm::vec3{parent->width / 2, parent->height / 2, 0.0f};
                    }
                    
                    result = getAttribMetaData(r, PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&parent->color)[j];
                        }             
                    }

                    result = getAttribMetaData(r, PONT_TEXCOORD);
                    
                    if (result.second >= 2 * sizeof(float))
                    {
                        orientation.x /= parent->width;
                        orientation.y /= parent->height;
                        for (int j = 0; j < 2; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = this->tex.source == nullptr ? 0.0 : this->tex.texCoords[j + i * 2];
                        }
                    }

                    result = getAttribMetaData(r, PONT_TEXID);
                    if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                    {
                        if (this->tex.source == nullptr)
                        {
                            *(float *)((char *)r.data + result.first + stride) = 0.0f;
                        }
                        else
                        {
                            *(float *)((char *)r.data + result.first + stride) = this->tex.source == nullptr ? 0.0 : this->tex.source->texID;
                        }
                    }
                    stride += getLayoutLen(r);
                }

                r.isDirty = true;
                
                return stride / (4 * getLayoutLen(r));
            }

            void SpriteRenderer::toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property)
            {
                int offset = rOffset * 4 * getLayoutLen(r);
                
                off_len result = getAttribMetaData(r, property);
                for (int i = 0; i < 4; i++)
                {

                    glm::vec3 orientation;
                    switch (i)
                    {
                        case 0: orientation = {1.0f * parent->width, 1.0f * parent->height, 0.0f}; break;
                        case 1: orientation = {0.0f * parent->width, 1.0f * parent->height, 0.0f}; break;
                        case 2: orientation = {0.0f * parent->width, 0.0f * parent->height, 0.0f}; break;
                        case 3: orientation = {1.0f * parent->width, 0.0f * parent->height, 0.0f}; break;
                    }

                    switch (property)
                    {
                        case PONT_POS:
                        {
                            if (result.second >= 3 * sizeof(float))
                            {
                                parent->pos += orientation - glm::vec3{parent->width / 2, parent->height / 2, 0.0f};

                                for (int j = 0; j < 3; j++)
                                {
                                    ((float *)((char *)r.data + result.first + offset))[j] = ((float *)&parent->pos)[j];
                                }

                                parent->pos -= orientation - glm::vec3{parent->width / 2, parent->height / 2, 0.0f};
                            }
                        } break;
                        case PONT_COLOR:
                        {
                            if (result.second >= 4 * sizeof(float))
                            {
                                for (int j = 0; j < 4; j++)
                                {
                                    ((float *)((char *)r.data + result.first + offset))[j] = ((float *)&parent->color)[j];
                                }
                            }
                        } break;
                        case PONT_TEXCOORD:
                        {
                            if (result.second >= 2 * sizeof(float))
                            {
                                orientation.x /= parent->width;
                                orientation.y /= parent->height;
                                for (int j = 0; j < 2; j++)
                                {
                                    *(float *)((char *)r.data + result.first + offset) = this->tex.texCoords[j + i * 2];
                                }
                            }
                        } break;
                        case PONT_TEXID:
                        {
                            __pMessage("Don't change the TexID!"); // at least not yet
                        }
                    }

                    offset += getLayoutLen(r);
                }

                r.isDirty = true;
            }
        }
    }
}