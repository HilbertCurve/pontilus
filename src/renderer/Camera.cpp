#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Application.h"
#include "renderer/Renderer.h"
#include "renderer/Camera.h"

namespace Pontilus
{
    namespace Renderer
    {
        Camera::Camera(Camera::CameraMode m) {
            this->mode = m;
            this->width = 80;
            this->height = 60;
        }

        glm::mat4 &Camera::getProjection()
        {
            if (mode == CameraMode::PERSPECTIVE)
            {
                projection = glm::perspective(90.0f, width / height, 0.0f, 100.0f);
            }
            else if (mode == CameraMode::ORTHOGONAL)
            {
                float left = -width / 20;
                float right = -left;

                float down = -height / 20;
                float up = -down;

                projection = glm::ortho(left, right, down, up, -10.0f, 100.0f);
            }

            return projection;
        }

        glm::mat4 &Camera::getView()
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
                glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)) * // pitch
                glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));  // yaw

            view = glm::inverse(transform);

            return view;
        }

        glm::vec3 &Camera::getPosition()
        {
            return this->position;
        }

        float Camera::getZoom()
        {
            return this->zoom;
        }

        void Camera::setMode(CameraMode m)
        {
            mode = m;
        }

        void Camera::move(float dx, float dy, float dz)
        {
            position += glm::vec3(dx, dy, dz);

            // for (int i = 0; i < 4; i++)
            // {
            //     for (int j = 0; j < 3; j++)
            //     {
            //         ((float *)fullWindowQuad.data)[i * 7 + j] += glm::vec3(dx, dy, dz)[j];
            //     }
            // }
        }

        void Camera::setPosition(float x, float y, float z)
        {
            position = glm::vec3{x, y, z};

            // for (int i = 0; i < 4; i++)
            // {
            //     for (int j = 0; j < 3; j++)
            //     {
            //         ((float *)fullWindowQuad.data)[i * 7 + j] = glm::vec3(x, y, z)[j];
            //     }
            // }
        }

        void Camera::rotate(float dpitch, float dyaw)
        {
            rotation += glm::vec3(dpitch, dyaw, 0);
        }

        void Camera::changeZoom(float dz)
        {
            zoom += dz;
        }
    }
}
