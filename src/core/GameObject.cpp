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
        using namespace Graphics;
        void gameStateToRend(GameObject &g, Rend &r, unsigned int rOffset)
        {
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

                    // makeshift memcpy(), because doing memcpy seems to 
                    // mess up malloc tables, or something.
                    // TODO: just use memcpy, it doesn't matter.
                    for (int i = 0; i < 3; i++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[i] = ((float *)&g.pos)[i];
                    }

                    g.pos -= orientation - glm::vec3{g.width / 2, g.height / 2, 0.0f};
                }
                
                result = getAttribMetaData(r, PONT_COLOR);
                if (result.second >= 4 * sizeof(float))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[i] = ((float *)&g.color)[i];
                    }             
                }

                result = getAttribMetaData(r, PONT_TEXCOORD);
                if (result.second >= 2 * sizeof(float))
                {
                    orientation.x /= g.width;
                    orientation.y /= g.height;
                    for (int i = 0; i < 2; i++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[i] = orientation[i];
                    }
                }

                result = getAttribMetaData(r, PONT_TEXID);
                if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                {
                    *(float *)((char *)r.data + result.first + stride) = 1.0f; // TODO: gameObject textures
                }
                stride += getLayoutLen(r);
            }
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
    }
}