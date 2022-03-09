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
            CameraMode mode = PONT_ORTHO;

            float projectionWidth = window.width / 10;
            float projectionHeight = window.height / 10;

            glm::mat4 &getProjection()
            {
                if (projectionMatrixIsDirty)
                {
                    if (mode == PONT_PERSPECTIVE)
                    {
                        camera.projection = glm::perspective(90.0f, projectionWidth / projectionHeight, 0.0f, 100.0f);
                    }
                    else if (mode == PONT_ORTHO)
                    {
                        float left = -projectionWidth / 2;
                        float right = -left;

                        float down = -projectionHeight / 2;
                        float up = -down;

                        camera.projection = glm::ortho(left, right, down, up, -10.0f, 100.0f);
                    }
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

            void setMode(CameraMode m)
            {
                mode = m;
                updateProjection();
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

            void setPosition(float x, float y, float z)
            {
                camera.position = glm::vec3{x, y, z};

                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        ((float *)fullWindowQuad.data)[i * 7 + j] = glm::vec3(x, y, z)[j];
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
