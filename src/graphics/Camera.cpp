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

            glm::mat4 &getProjection()
            {
                static bool hasInit = false;
                if (!hasInit)
                {
                    camera.projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -10.0f, 10000.0f);
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
