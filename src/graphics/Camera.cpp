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
                    camera.projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 15.0f);
                }

                return camera.projection;
            }

            glm::mat4 &getView()
            {
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), camera.position) *
                                    glm::rotate(glm::mat4(1.0f), camera.rotation.x, glm::vec3(1, 0, 0)) * // pitch
                                    glm::rotate(glm::mat4(1.0f), camera.rotation.y, glm::vec3(0, 1, 0)) * // yaw
                                    glm::rotate(glm::mat4(1.0f), camera.rotation.z, glm::vec3(0, 0, 1));  // roll

                camera.view = glm::inverse(transform);

                return camera.view;
            }
        }
    }
}
