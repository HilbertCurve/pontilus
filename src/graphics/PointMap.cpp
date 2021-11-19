#include "postprocessing/PointMap.h"

#include <glm/glm.hpp>
#include <random>

#include "Application.h"
#include "InputListener.h"
#include "Utils.h"

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
                    ((float *)pointLightPool.data)[i * 8 + j] = 0.0f;
                }

                // color
                for (int j = 0; j < 4; j++)
                {
                    ((float *)pointLightPool.data)[i * 8 + j + 3] = 1.0f;
                }

                // intensity
                ((float *)pointLightPool.data)[i * 8 + 7] = 0.1f;
            }
        }

        void updatePointMap(double dt)
        {
            for (int i = 0; i < 3; i++)
            {
                ((float *)pointLightPool.data)[i * 8] = cos(glfwGetTime());
                ((float *)pointLightPool.data)[i * 8 + 1] = sin(glfwGetTime());
            }

            ((float *)pointLightPool.data)[3 * 8] = screenToWorldCoords(IO::mousePos()).x;
            ((float *)pointLightPool.data)[3 * 8 + 1] = screenToWorldCoords(IO::mousePos()).y;
        }
    }
}