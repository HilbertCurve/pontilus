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
                switch (i)
                {
                    case 0: 
                    {

                        ((float *)pointLightPool.data)[i * 8 + 3] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 4] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 5] = 1.0f;
                        ((float *)pointLightPool.data)[i * 8 + 6] = 0.5f;
                    } break;
                    case 1:
                    {
                        ((float *)pointLightPool.data)[i * 8 + 3] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 4] = 1.0f;
                        ((float *)pointLightPool.data)[i * 8 + 5] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 6] = 0.5f;
                    } break;
                    case 2:
                    {
                        ((float *)pointLightPool.data)[i * 8 + 3] = 1.0f;
                        ((float *)pointLightPool.data)[i * 8 + 4] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 5] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 6] = 0.5f;
                    }
                    case 3:
                    {
                        ((float *)pointLightPool.data)[i * 8 + 3] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 4] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 5] = 0.0f;
                        ((float *)pointLightPool.data)[i * 8 + 6] = 0.0f;
                    }
                    
                }
                    ((float *)pointLightPool.data)[i * 8] = i * 2;

                // intensity
                ((float *)pointLightPool.data)[i * 8 + 7] = 0.5f;
            }
        }

        void updatePointMap(double dt)
        {
            static float f = 0.0f;
            for (int i = 0; i < 4; i++)
            {
                ((float *)pointLightPool.data)[i * 8 + 0] = cos(glfwGetTime()) * ((i % 2) - 0.5f) * 2;
                ((float *)pointLightPool.data)[i * 8 + 1] = sin(glfwGetTime()) * ((i % 2) - 0.5f) * 2;

                // shoddy color setting
                // TODO: RData-specific value setting
                
            }
        }
    }
}