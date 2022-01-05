#include "core/Scene.h"

#include <glm/glm.hpp>

#include "core/Application.h"
#include "core/InputListener.h"
#include "ecs/GameObject.h"
#include "ecs/SpriteRenderer.h"
#include "graphics/rData.h"
#include "graphics/Font.h"
#include "graphics/Camera.h"

namespace Pontilus
{
    namespace Engine
    {
        static ECS::GameObject *g1 = new ECS::GameObject();
        static ECS::GameObject *g2 = new ECS::GameObject();
        static ECS::GameObject *g3 = new ECS::GameObject();
        static ECS::GameObject *g4 = new ECS::GameObject();
        static Graphics::IconMap im1;
        static Graphics::IconMap im2;
        static Graphics::IconMap blueberry;
        static Graphics::IconMap burger;
        static Graphics::IconMap pizzaMonster;
        static Graphics::Font jetBrainsMono;
        static Scene s = {};

        void init()
        {
            s.init = []()
            {
                g1->init({-7.5f, 0.0f, 0.0f }, { 0.0f, 0.1f, 0.5f, 1.0f }, 3.0f, 3.0f);

                g2->init({-2.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                g3->init({ 2.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                g4->init({ 7.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);

                Graphics::initIconMap("./assets/images/test.png", im1, 8, 8, 0);
                Graphics::initIconMap("./assets/images/test2.png", im2, 8, 8, 0);
                Graphics::initIconMap("./assets/images/blueberry.png", blueberry, 32, 32, 0);
                Graphics::initIconMap("./assets/images/burger.png", burger, 32, 32, 0);
                Graphics::initIconMap("./assets/images/pizzaMonster.png", pizzaMonster, 32, 32, 0);

                Graphics::initFont(jetBrainsMono, "./assets/fonts/JetBrainsMono-Medium.ttf", 32);

                //s.objs.push_back(g1);
                //s.objs.push_back(g2);
                //s.objs.push_back(g3);
                //s.objs.push_back(g4);
                //s.objs.push_back(t1);

                for (int i = 0; i < s.objs.size(); i++)
                {
                    try
                    {
                        ECS::Component &csr = s.objs.at(i).getComponent(typeid(ECS::SpriteRenderer()));
                        ECS::SpriteRenderer sr = dynamic_cast<ECS::SpriteRenderer &>(csr);

                        // todo
                    }
                    catch (std::exception)
                    {
                        continue;
                    }
                }
            };

            s.update = [](double dt)
            {
                /*
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
                        printf("(%f, %f)\n", g2.pos.x, g2.pos.y);
                        keyIsPressed1 = keyIsPressed0 = true;
                    }
                    atRestY = false;
                }
                else
                {
                    keyIsPressed1 = keyIsPressed0 = false;
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
                // move camera
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

                //g1.toRData(quadPool, 0, Graphics::PONT_POS);
            };

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