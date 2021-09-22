#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Application.h"

namespace Pontilus
{
    namespace Renderer
    {
        glm::mat4 &Camera::getProjection()
        {
            static bool hasInit = false;
            if (!hasInit)
            {
                this->projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.0f, 15.0f);
            }

            return this->projection;
        }

        glm::mat4 &Camera::getView()
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->position) *
                                  glm::rotate(glm::mat4(1.0f), this->rotation.x, glm::vec3(1, 0, 0)) * // pitch
                                  glm::rotate(glm::mat4(1.0f), this->rotation.y, glm::vec3(0, 1, 0)) * // yaw
                                  glm::rotate(glm::mat4(1.0f), this->rotation.z, glm::vec3(0, 0, 1));  // roll

            this->view = glm::inverse(transform);

            return view;
        }
    }
}
