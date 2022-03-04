#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Application.h"
#include "graphics/Camera.h"

namespace Pontilus
{
    namespace Renderer
    {
        namespace Camera
        {
            static _Camera camera = _Camera();
            static bool projectionMatrixIsDirty = true;

            float projectionWidth = window.width / 10;
            float projectionHeight = window.height / 10;

            glm::mat4 &getProjection()
            {
                if (projectionMatrixIsDirty)
                {
                    camera.projection = glm::perspective(90.0f, projectionWidth / projectionHeight, 0.0f, 100.0f);
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

            float getZoom()
            {
                return camera.zoom;
            }

            void updateProjection()
            {
                projectionWidth = window.width / 10;
                projectionHeight = window.height / 10;

                projectionMatrixIsDirty = true;
            }

            void move(float dx, float dy, float dz)
            {
                camera.position += glm::vec3(dx, dy, dz);

                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        ((float *)fullWindowQuad.data)[i * 7 + j] += glm::vec3(dx, dy, dz)[j];
                    }
                }
            }
            void rotate(float dpitch, float dyaw)
            {
                camera.rotation += glm::vec3(dpitch, dyaw, 0);
            }
        }
    }
}
