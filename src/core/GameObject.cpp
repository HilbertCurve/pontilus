#include "GameObject.h"

#include <string.h>
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

                pos_len info = getAttribMetaData(r, PONT_POS);
                if (info.second >= 3 * sizeof(float))
                {
                    g.pos += orientation;
                    memcpy((char *)(info.first) + stride, &g.pos, 3 * sizeof(float));
                    g.pos -= orientation;
                }
                info = getAttribMetaData(r, PONT_COLOR);
                if (info.second >= 4 * sizeof(float))
                {
                    memcpy((char *)(info.first) + stride, &g.color, 4 * sizeof(float));
                }

                stride += getLayoutLen(r);
            }
        }
    }
}