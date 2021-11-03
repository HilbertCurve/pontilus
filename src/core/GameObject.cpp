#include "GameObject.h"

#include <string.h>
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "Rend.h"
#include "Utils.h"

namespace Pontilus
{
    namespace Engine
    {
        void initGameObject(GameObject &g, glm::vec3 pos, glm::vec4 color, float width, float height)
        {
            g.pos = pos;
            g.color = color;
            g.width = width;
            g.height = height;
        }

        using namespace Graphics; // fight me
        void gameStateToRend(GameObject &g, Rend &r, unsigned int rOffset)
        {
            __pAssert(!(rOffset >= r.vertCount / 4), "Rend not big enough to hold game states!");

            static int texID = 1;
            int stride = rOffset * getLayoutLen(r) * 4;
            for (int i = 0; i < 4; i++)
            {
                glm::vec3 orientation;
                switch (i)
                {
                    case 0: orientation = {1.0f * g.width, 1.0f * g.height, 0.0f}; break;
                    case 1: orientation = {0.0f * g.width, 1.0f * g.height, 0.0f}; break;
                    case 2: orientation = {0.0f * g.width, 0.0f * g.height, 0.0f}; break;
                    case 3: orientation = {1.0f * g.width, 0.0f * g.height, 0.0f}; break;
                }

                off_len result = getAttribMetaData(r, PONT_POS);
                if (result.second >= 3 * sizeof(float))
                {
                    g.pos += orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f};

                    // TODO: just use memcpy, bonehead.
                    for (int j = 0; j < 3; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&g.pos)[j];
                    }

                    g.pos -= orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f};
                }
                
                result = getAttribMetaData(r, PONT_COLOR);
                if (result.second >= 4 * sizeof(float))
                {
                    for (int j = 0; j < 4; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&g.color)[j];
                    }             
                }

                result = getAttribMetaData(r, PONT_TEXCOORD);
                if (result.second >= 2 * sizeof(float))
                {
                    orientation.x /= g.width;
                    orientation.y /= g.height;
                    for (int j = 0; j < 2; j++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[j] = orientation[j];
                    }
                }

                result = getAttribMetaData(r, PONT_TEXID);
                if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                {
                    *(float *)((char *)r.data + result.first + stride) = texID; // TODO: gameObject textures
                }
                stride += getLayoutLen(r);
            }

            texID++;

            r.isDirty = true;
        }

        void gameStateToRend(std::vector<GameObject> gs, Rend &r)
        {
            __pAssert(r.vertCount >= 4 * gs.size(), "Rend not big enough to hold game states!");

            int stride = 0;
            for (GameObject g : gs)
            {
                gameStateToRend(g, r, stride);
                stride += getLayoutLen(r) * 4;
            }
        }

        void gameStateToRend(GameObject &g, Rend &r, unsigned int rOffset, vProp property)
        {
            int offset = rOffset * 4 * getLayoutLen(r);
            
            off_len result = getAttribMetaData(r, property);
            for (int i = 0; i < 4; i++)
            {

                glm::vec3 orientation;
                switch (i)
                {
                    case 0: orientation = {1.0f * g.width, 1.0f * g.height, 0.0f}; break;
                    case 1: orientation = {0.0f * g.width, 1.0f * g.height, 0.0f}; break;
                    case 2: orientation = {0.0f * g.width, 0.0f * g.height, 0.0f}; break;
                    case 3: orientation = {1.0f * g.width, 0.0f * g.height, 0.0f}; break;
                }

                switch (property)
                {
                    case PONT_POS:
                    {
                        if (result.second >= 3 * sizeof(float))
                        {
                            g.pos += orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f};

                            for (int j = 0; j < 3; j++)
                            {
                                ((float *)((char *)r.data + result.first + offset))[j] = ((float *)&g.pos)[j];
                            }

                            g.pos -= orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f};
                        }
                    } break;
                    case PONT_COLOR:
                    {
                        if (result.second >= 4 * sizeof(float))
                        {
                            for (int j = 0; j < 4; j++)
                            {
                                ((float *)((char *)r.data + result.first + offset))[j] = ((float *)&g.color)[j];
                            }
                        }
                    } break;
                    case PONT_TEXCOORD:
                    {
                        if (result.second >= 2 * sizeof(float))
                        {
                            orientation.x /= g.width;
                            orientation.y /= g.height;
                            for (int j = 0; j < 2; j++)
                            {
                                ((float *)((char *)r.data + result.first + offset))[j] = orientation[j];
                            }
                        }
                    } break;
                    case PONT_TEXID:
                    {
                        __pMessage("Don't change the TexID of a gameObject!");
                    }
                }

                offset += getLayoutLen(r);
            }

            r.isDirty = true;
        }
    }
}