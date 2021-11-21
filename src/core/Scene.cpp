#include "Scene.h"

#include <glm/glm.hpp>

#include "Application.h"
#include "GameObject.h"
#include "rData.h"
#include "InputListener.h"
#include "Camera.h"

namespace Pontilus
{
    namespace Engine
    {
        static GameObject g1;
        static Scene s = 
        {
            []()
            {
                g1 = {};
                initGameObject(g1, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g1.tex);
                s.objs.push_back(g1);

                for (int i = 0; i < s.objs.size(); i++)
                {
                    gameStateToRData(s.objs[i], quadPool, i);
                }
            },
            [](double dt)
            {
                static bool keyIsPressed0 = false;
                static bool keyIsPressed1 = false;
                static bool atRestY = false;

                static glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };
                static glm::vec3 gravity = { 0.0f, -9.8f, 0.0f };
                static glm::vec3 drag = { 0.0f, 0.0f, 0.0f };
                static glm::vec3 acceleration = { 0.0f, 0.0f, 0.0f };
                static const float floorY = -7.0f;
                static const float dragConst = 1.75f;

                static float camToPlayer;
                static float camVelocity;

                drag = velocity * dragConst;
                
                if (0.001f > drag.x && drag.x > -0.001f)
                {
                    drag.x = 0.0f;
                }
                
                // jumping
                if (IO::isKeyPressed(GLFW_KEY_SPACE) && atRestY)
                {
                    keyIsPressed0 = true;
                    if (!(keyIsPressed0 == keyIsPressed1))
                    {
                        velocity.y += 7.0f;
                        keyIsPressed1 = keyIsPressed0 = true;
                    }
                    atRestY = false;
                }
                else
                {
                    keyIsPressed1 = keyIsPressed1 = false;
                }

                // side-to-side motion (right has precedence over left)
                if (IO::isKeyPressed(GLFW_KEY_D))
                {
                    acceleration.x = 5.0f;
                }
                else if (IO::isKeyPressed(GLFW_KEY_A))
                {
                    acceleration.x = -5.0f;
                }
                else
                {
                    acceleration.x = 0.0f;
                }

                // hitting the floor
                glm::vec3 dpos = velocity * (float) dt;
                if (g1.pos.y + dpos.y > floorY) // if the next frame won't put me in the floor...
                {
                    // impulse move
                    g1.pos += velocity * (float) dt;
                    velocity += (gravity + acceleration - drag) * (float) dt;
                }
                else
                {
                    // otherwise snap me to the floor
                    velocity += (acceleration - drag) * (float) dt;
                    velocity.y = 0.0f;
                    g1.pos += velocity * (float) dt;
                    g1.pos.y = floorY;
                    atRestY = true;
                }

                if (IO::isKeyPressed(GLFW_KEY_RIGHT))
                {
                    Renderer::Camera::move(0.1f, 0.0f, 0.0f);
                }
                if (IO::isKeyPressed(GLFW_KEY_LEFT))
                {
                    Renderer::Camera::move(-0.1f, 0.0f, 0.0f);
                }

                /*
                // update camera pos and move it accordingly
                camToPlayer = abs(Renderer::Camera::getPosition().x - g1.pos.x);
                if (camToPlayer > 8.5f)
                {
                    camVelocity += 0.1f;
                    Renderer::Camera::move(camVelocity / dt, 0, 0);
                }
                else
                {
                    if (camVelocity - 0.3f !)
                    camVelocity -= 0.3f;
                    Renderer::Camera::move(camVelocity / dt, 0, 0);
                }
                */

                gameStateToRData(g1, quadPool, 0, Graphics::PONT_POS);
            }
        };

        void init()
        {
            s.init();
        }

        void update(double dt)
        {
            s.update(dt);
        }

        Scene *getScene()
        {
            return &s;
        }
    }
}