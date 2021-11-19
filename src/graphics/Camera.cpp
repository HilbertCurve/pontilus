#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Application.h"

namespace Pontilus
{
    namespace Renderer
    {
        namespace Camera
        {
            static _Camera camera = _Camera();
            static bool projectionMatrixIsDirty = true;

            float projectionWidth = 20.0f;
            float projectionHeight = 20.0f;

            glm::mat4 &getProjection()
            {
                if (projectionMatrixIsDirty)
                {
                    float left = -projectionWidth / 2;
                    float right = -left;

                    float down = -projectionHeight / 2;
                    float up = -down;

                    camera.projection = glm::ortho(left, right, down, up, -10.0f, 100.0f);
                }

                return camera.projection;
            }

            glm::mat4 &getView()
            {
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera.position) * 
                    glm::rotate(glm::mat4(1.0f), camera.rotation.x, glm::vec3(1, 0, 0)) * // pitch
                    glm::rotate(glm::mat4(1.0f), camera.rotation.y, glm::vec3(0, 1, 0));  // yaw

                camera.view = glm::inverse(transform);

                return camera.view;
            }

            glm::vec3 &getPosition()
            {
                return camera.position;
            }

            void move(float dx, float dy, float dz)
            {
                camera.position += glm::vec3(dx, dy, dz);
            }
            void rotate(float dpitch, float dyaw)
            {
                camera.rotation += glm::vec3(dpitch, dyaw, 0);
            }
        }
    }
}
