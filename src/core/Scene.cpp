#include "core/Scene.h"

#include <glm/glm.hpp>
#include <string>

#include "audio/AudioMaster.h"
#include "audio/AudioListener.h"
#include "audio/AudioSource.h"
#include "core/Application.h"
#include "core/InputListener.h"
#include "ecs/GameObject.h"
#include "renderer/TextRenderer.h"
#include "renderer/SpriteRenderer.h"
#include "renderer/Renderer.h"
#include "renderer/rData.h"
#include "renderer/Font.h"
#include "renderer/Camera.h"
#include "library/Animation.h"
#include "model/ModelRenderer.h"

namespace Pontilus
{
    /*
    static ECS::GameObject g1 = ECS::GameObject(); 
    static ECS::GameObject g2 = ECS::GameObject();
    static ECS::GameObject g3 = ECS::GameObject();
    static ECS::GameObject g4 = ECS::GameObject();
    static ECS::SpriteRenderer s1;
    static ECS::SpriteRenderer s2;
    static Renderer::IconMap im1;
    static Renderer::IconMap im2;
    static Renderer::IconMap blueberry;
    static Renderer::IconMap burger;
    static Renderer::IconMap pizzaMonster;
    static Renderer::Font jetBrainsMono;

    static ECS::GameObject leftPaddle, rightPaddle, ball, lScore, rScore;
    static ECS::SpriteRenderer lPaddleRenderer, rPaddleRenderer, ballRenderer;
    static ECS::TextRenderer lScoreText, rScoreText;
    static Renderer::IconMap pogFace;

    static ECS::GameObject toBeAnimated;
    static ECS::SpriteRenderer toBeAnimatedSpr;
    static ECS::Animation animator;
    static Renderer::IconMap toBeAnimatedFrames;

    static ECS::GameObject theListener;
    static ECS::GameObject theNoiseGenerator;
    static ECS::AudioSource theSource;
    static Audio::WAVFile *theNoise = new Audio::WAVFile();
        */

    ECS::GameObject &Scene::spawn()
    {
        ECS::GameObject *obj = new ECS::GameObject();
        obj->currentScene = this;
        this->objs.emplace_back(obj);
        return *this->objs.back();
    }

    ECS::GameObject &Scene::get(int id) {
        for (int i = 0; i < this->objs.size(); i++)
        {
            ECS::GameObject *gameObject = this->objs[i];
            if (gameObject->id == id)
            {
                return *gameObject;
            }
        }

        __pError("Could not find GameObject of id %u.", id);
    }

    void Scene::despawn(int id)
    {
        for (int i = 0; i < this->objs.size(); i++)
        {
            ECS::GameObject *gameObject = this->objs[i];
            if (gameObject->id == id)
            {
                this->objs.erase(this->objs.begin() + i);
                delete gameObject;
            }
        }
    }

    void Scene::updateObjects(double dt)
    {
        for (int i = 0; i < this->objs.size(); i++)
        {
            this->objs.at(i)->update(dt);
        }
    }

    void Scene::freeObjects()
    {
        for (ECS::GameObject *obj : this->objs) {
            delete obj;
        }
        this->objs.clear();
    }
    
    // tests
    /*
    Scene Scenes::audioTest =
    {
        []()
        {
            Audio::initWAVFile(*theNoise, "../assets/sounds/test2.wav");

            theSource.init();

            theNoiseGenerator.init({0.0f, 0.0f, 0.0f}, 0.0f, 0.0f);
            theNoiseGenerator.addComponent(theSource);
            /*
            IO::submitKeyCallback([](int key, int action)
            {
                if (key == GLFW_KEY_SPACE && action != GLFW_RELEASE)
                {
                    printf("bar");
                    theSource.play(theNoise, false);
                }
            });
                
        },
        [](double dt)
        {
            static bool keyIsPressed0 = false;
            static bool keyIsPressed1 = false;
            if (IO::isKeyPressed(GLFW_KEY_SPACE))
            {
                keyIsPressed0 = true;
                if (!(keyIsPressed0 == keyIsPressed1))
                {
                    theSource.play(*theNoise, false);
                    keyIsPressed1 = keyIsPressed0 = true;
                }
            }
            else
            {
                keyIsPressed1 = false;
            }
        },
        []()
        {
            delete theNoise;
        }
    };
*/
/*
    Scene Scenes::animation =
    {
        []()
        {
            Renderer::initIconMap("../assets/textures/test2.png", toBeAnimatedFrames, 8, 8, 0);

            toBeAnimated = ECS::GameObject();
            toBeAnimatedSpr = ECS::SpriteRenderer();
            animator = ECS::Animation();

            toBeAnimated.init({0.0, 0.0, 0.0}, 10, 10);
            
            animator.init(toBeAnimatedFrames, 0, 3, true);
            animator.setAnimationBounds(0, 3);

            toBeAnimatedSpr.tex = animator.textures.at(0);

            toBeAnimated.addComponent(toBeAnimatedSpr);
            toBeAnimated.addComponent(animator);

            animation.objs.push_back(toBeAnimated);
        },
        [](double dt)
        {
            static double time = 0.0;
            time += dt;

            if (time > 0.5)
            {
                animator.next();

                time = 0.0;
            }
        },
        []()
        {

        }
    };

*/
/*
    Scene Scenes::pog = 
    {
        []()
        {
            Renderer::initIconMap("../assets/textures/pogFace.png", pogFace, 200, 200, 0);
            
            Renderer::initFont(jetBrainsMono, "../assets/fonts/JetBrainsMono-Medium.ttf", 32);

            leftPaddle = rightPaddle = ball = ECS::GameObject();
            lScore = rScore = ECS::GameObject();
            lPaddleRenderer = rPaddleRenderer = ballRenderer = ECS::SpriteRenderer();
            lScoreText = rScoreText = ECS::TextRenderer();

            leftPaddle.init({-30.0, 0.0, 0.0}, 1.0, 8.0);
            rightPaddle.init({30.0, 0.0, 0.0}, 1.0, 8.0);
            ball.init({0.0, 0.0, 0.0}, 2.0, 2.0);
            lScore.init({-30.0, -17.0, 0.0}, 3.0, 3.0);
            rScore.init({30.0, -17.0, 0.0}, 3.0, 3.0);
            
            ballRenderer.tex = Renderer::getTexture(pogFace, 0);

            lScoreText.init("0", jetBrainsMono, {1.0f, 1.0f, 1.0f, 1.0f});
            rScoreText.init("0", jetBrainsMono, {1.0f, 1.0f, 1.0f, 1.0f});

            leftPaddle.addComponent(lPaddleRenderer);
            rightPaddle.addComponent(rPaddleRenderer);
            ball.addComponent(ballRenderer);
            lScore.addComponent(lScoreText);
            rScore.addComponent(rScoreText);

            pog.objs.push_back(leftPaddle);
            pog.objs.push_back(rightPaddle);
            pog.objs.push_back(ball);
            pog.objs.push_back(lScore);
            pog.objs.push_back(rScore);
        },
        [](double dt)
        {
            static const float gameHeight = 30.0;
            static float speed = 27.0;

            static glm::vec2 pogVelocity = glm::vec2{(float) cos(50), (float) sin(50)} * speed;
            static int lScoreNum = 0, rScoreNum = 0;

            // update positions
            if (IO::isKeyPressed(GLFW_KEY_UP) && rightPaddle.pos.y <= gameHeight / 2)
            {
                rightPaddle.pos.y += 0.2f;
            }
            else if (IO::isKeyPressed(GLFW_KEY_DOWN) && rightPaddle.pos.y >= -gameHeight / 2)
            {
                rightPaddle.pos.y -= 0.2f;
            }

            if (IO::isKeyPressed(GLFW_KEY_W) && leftPaddle.pos.y <= gameHeight / 2)
            {
                leftPaddle.pos.y += 0.2f;
            }
            else if (IO::isKeyPressed(GLFW_KEY_S) && leftPaddle.pos.y >= -gameHeight / 2)
            {
                leftPaddle.pos.y -= 0.2f;
            }

            if (ball.pos.y + pogVelocity.y * dt >= gameHeight / 2 || ball.pos.y + pogVelocity.y * dt <= -gameHeight / 2)
            {
                pogVelocity.y *= -1;
            }

            // check if ball is past left/right threshold
            if (ball.pos.x + (ball.width + rightPaddle.width) / 2 / 2 >= rightPaddle.pos.x && pogVelocity.x >= 0)
            {
                // check if ball is touching paddle
                if (ball.pos.y < rightPaddle.pos.y + rightPaddle.height / 2 + ball.height / 2 &&
                    ball.pos.y > rightPaddle.pos.y - rightPaddle.height / 2 - ball.height / 2)
                {
                    pogVelocity.x *= -1.0;
                    pogVelocity.y = sin(glfwGetTime()) * speed;

                    speed += 0.5f;
                }
                else
                {
                    ball.pos = {0.0, 0.0, 0.0};
                    speed = 17.0;
                    lScoreNum++;
                }
            }

            if (ball.pos.x - (ball.width + leftPaddle.width) / 2 <= leftPaddle.pos.x && pogVelocity.x <= 0)
            {
                // check if ball is touching paddle
                if (ball.pos.y < leftPaddle.pos.y + leftPaddle.height / 2 + ball.height / 2 &&
                    ball.pos.y > leftPaddle.pos.y - leftPaddle.height / 2 - ball.height / 2)
                {
                    pogVelocity.x *= -1.0;
                    pogVelocity.y = sin(glfwGetTime()) * speed;

                    speed == 0.5;
                }
                else
                {
                    ball.pos = {0.0f, 0.0f, 0.0f};
                    speed = 17.0;
                    rScoreNum++;
                }
            }

            ball.pos += glm::vec3(pogVelocity * (float) dt, 0.0);

            lScoreText.text = std::to_string(lScoreNum);
            rScoreText.text = std::to_string(rScoreNum);
        },
        []()
        {
            
        }
    };
*/
/*
    Scene Scenes::debug = 
    {
        []() 
        {
            g1.init({-7.5f, 0.0f, 0.0f }, 3.0f, 3.0f);

            g2.init({-2.5f, 0.0f, 0.0f }, 3.0f, 3.0f);

            g3.init({ 2.5f, 0.0f, 0.0f }, 3.0f, 3.0f);

            g4.init({ 7.5f, 0.0f, 0.0f }, 3.0f, 3.0f);

            Renderer::initIconMap("../assets/textures/test.png", im1, 8, 8, 0);
            Renderer::initIconMap("../assets/textures/test2.png", im2, 8, 8, 0);
            Renderer::initIconMap("../assets/textures/blueberry.png", blueberry, 32, 32, 0);
            Renderer::initIconMap("../assets/textures/burger.png", burger, 32, 32, 0);
            Renderer::initIconMap("../assets/textures/pizzaMonster.png", pizzaMonster, 32, 32, 0);

            Renderer::initFont(jetBrainsMono, "../assets/fonts/JetBrainsMono-Medium.ttf", 32);

            s1.tex = Renderer::getTexture(blueberry, 0);
            s2.tex = Renderer::getTexture(burger, 0);

            g1.addComponent(s1);
            g2.addComponent(s2);

            debug.objs.push_back(g1);
            debug.objs.push_back(g2);
            //s.objs.push_back(g3);
            //s.objs.push_back(g4);
            //s.objs.push_back(t1);
        },
        [](double dt)
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
            

            //g1.toRData(quadPool, 0, Renderer::PONT_POS);
        },
        []()
        {

        }
    };
    */
}

