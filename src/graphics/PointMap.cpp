#include "postprocessing/PointMap.h"

#include <glm/glm.hpp>
#include <random>

#include "Application.h"

namespace Pontilus
{
    namespace Graphics
    {
        void initPointMap()
        {
            for (int i = 0; i < 4; i++)
            {
                // position
                for (int j = 0; j < 3; j++)
                {
                    ((float *)pointLightPool.data)[i * 8 + j] = 100.0f;
                }

                // color
                for (int j = 0; j < 4; j++)
                {
                    ((float *)pointLightPool.data)[i * 8 + j + 3] = 1.0f;
                }

                // intensity
                ((float *)pointLightPool.data)[i * 8 + 7] = 0.2f;
            }
        }

        void updatePointMap(double dt)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    ((float *)pointLightPool.data)[i * 8 + j] += 1.0f;
                }
            }
        }
    }
}