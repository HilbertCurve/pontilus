/* date = August 30th 2021 9:08 pm */

#ifndef _PONTILUS_CAMERA_H
#define _PONTILUS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pontilus
{
    namespace Renderer
    {
        namespace Camera
        {
            extern float projectionWidth;
            extern float projectionHeight;

            // TOTAL MEMORY: 1248 bits
            struct _Camera
            {
                glm::vec3 position = glm::vec3{0, 0, 0};
                // Vec3{pitch (x-axis), yaw (y-axis), roll (z-axis)}
                glm::vec3 rotation = glm::vec3{0, 0, 0};
                float fov = 90; // is this necessary? i'd like to implement it at some point.

                // these should NEVER be accessed directly (unless you know what you're doing)
                glm::mat4 projection;
                glm::mat4 view;
            };

            glm::mat4 &getProjection();
            glm::mat4 &getView();
            glm::vec3 &getPosition();

            void move(float dx, float dy, float dz);
            void rotate(float dpitch, float dyaw);
        }
    }
}

#endif //_PONTILUS_CAMERA_H
