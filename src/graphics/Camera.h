/* date = August 30th 2021 9:08 pm */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pontilus
{
    namespace Renderer
    {
        namespace Camera
        {
            enum CameraMode {
                PONT_ORTHO,
                PONT_PERSPECTIVE
            };

            extern float projectionWidth;
            extern float projectionHeight;

            struct _Camera
            {
                glm::vec3 position = glm::vec3{0, 0, 0};
                // Vec3{pitch (x-axis), yaw (y-axis), roll (z-axis)}
                glm::vec3 rotation = glm::vec3{0, 0, 0};
                float fov = 90; // is this necessary? i'd like to implement it at some point.
                float zoom = 15;

                // these should NEVER be accessed directly (unless you know what you're doing)
                glm::mat4 projection;
                glm::mat4 view;
            };

            glm::mat4 &getProjection();
            glm::mat4 &getView();
            glm::vec3 &getPosition();
            float getZoom();
            void updateProjection();
            void setMode(CameraMode m);

            void move(float dx, float dy, float dz);
            void setPosition(float x, float y, float z);
            void rotate(float dpitch, float dyaw);
        }
    }
}
