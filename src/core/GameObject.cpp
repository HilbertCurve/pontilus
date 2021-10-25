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
                    case 0: orientation = {1.0f, 1.0f, 0.0f}; break;
                    case 1: orientation = {0.0f, 1.0f, 0.0f}; break;
                    case 2: orientation = {0.0f, 0.0f, 0.0f}; break;
                    case 3: orientation = {1.0f, 0.0f, 0.0f}; break;
                }

                getAttribMetaData(r, PONT_POS);
                if (result.second >= 3 * sizeof(float))
                {
                    g.pos += orientation;
                    memcpy((char *)(result.first) + stride, (void *)&(g.pos), sizeof(g.pos));
                    g.pos -= orientation;
                }
                printf("hello\n");
                getAttribMetaData(r, (vProp)0);
                printf("hello2\n");
                if (result.second >= 4 * sizeof(float))
                {
                    printf("hello3\n");
                    memcpy((char *)(result.first) + stride, (void *)&(g.color), sizeof(g.color));
                    printf("hello4\n");                
                }
                printf("hello5\n");

                stride += getLayoutLen(r);
            }
        }
    }
}