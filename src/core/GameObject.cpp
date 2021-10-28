#include "GameObject.h"

#include <string.h>
#include <stdio.h>
#include <glm/glm.hpp>

#include "Rend.h"

namespace Pontilus
{
    namespace Engine
    {
        using namespace Graphics;
        void gameStateToRend(GameObject &g, Rend &r)
        {
            int stride = 0;
            for (int i = 0; i < 4; i++)
            {
                glm::vec3 orientation;
                switch (i)
                {
                    case 0: orientation = {15.0f, 15.0f, 0.0f}; break;
                    case 1: orientation = {0.0f, 15.0f, 0.0f}; break;
                    case 2: orientation = {0.0f, 0.0f, 0.0f}; break;
                    case 3: orientation = {15.0f, 0.0f, 0.0f}; break;
                }

                off_len result = getAttribMetaData(r, PONT_POS);
                if (result.second >= 3 * sizeof(float))
                {
                    g.pos += orientation;

                    // makeshift memcpy(), because doing memcpy seems to 
                    // mess up malloc tables, or something.

                    for (int i = 0; i < 3; i++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[i] = ((float *)&g.pos)[i];
                    }
                    g.pos -= orientation;
                    
                }
                
                result = getAttribMetaData(r, PONT_COLOR);
                
                if (result.second >= 4 * sizeof(float))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        ((float *)((char *)r.data + result.first + stride))[i] = ((float *)&g.color)[i];
                    }             
                }
                stride += getLayoutLen(r);
            }
        }
    }
}