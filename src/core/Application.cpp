#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/Application.h"
#include "core/Scene.h"
#include "core/InputListener.h"
#include "audio/AudioMaster.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "graphics/rData.h"
#include "graphics/Font.h"
#include "graphics/Texture.h"
#include "ecs/StateMachine.h"
#include "ecs/TextRenderer.h"
#include "ecs/SpriteRenderer.h"
#include "ecs/Transform.h"

// TODO: there really should be a single struct `Application` that holds a lot of the static stuff in this engine.
namespace Pontilus
{
    static _PONTILUS_SETTINGS args = 0x0000;

    float resolution = 512;

    Engine::Scene defaultScene = 
        {
            []()
            {
                static Renderer::IconMap defaultMap = Renderer::IconMap();
                static Renderer::Font jetBrainsMono;
                Engine::ECS::GameObject &defaultMessage = defaultScene.spawn();
                defaultMessage.addComponent(new Engine::ECS::Transform{{0.0, 0.0, 0.0}, {40.0, 50.0, 1.0}, {0.0, 0.0, 0.0}});
                Engine::ECS::GameObject &defaultLogo = defaultScene.spawn();
                defaultLogo.addComponent(new Engine::ECS::Transform{{0.0, -20.0, 0.0}, {20.0, 16.0, 1.0}, {0.0, 0.0, 0.0}});
                
                Renderer::initFont(jetBrainsMono, "../assets/fonts/JetBrainsMono-Medium.ttf", 26);
                Renderer::initIconMap("../assets/textures/ghostSwole.png", defaultMap, 675, 570, 0);
                
                Renderer::Texture t = Renderer::getTexture(defaultMap, 0);

                defaultMessage.addComponent(new Engine::ECS::TextRenderer(
                    "Whoops! The Scene hasn't been specified yet. Make sure to call pontilus.set_scene(s) before pontilus.loop().",
                    jetBrainsMono, {1.0f, 1.0f, 1.0f, 1.0f}));
                defaultLogo.addComponent(new Engine::ECS::SpriteRenderer(t, {1.0f, 1.0f, 1.0f, 1.0f}));
            },
            [](double dt)
            {

            },
            []()
            {

            }
        };


    // i'd prefer to keep this private; there are some quirks with getting and setting this variable i'd rather automate
    static Engine::Scene *currentScene;

    Engine::Scene *getCurrentScene()
    {
        return currentScene;
    }

    void setCurrentScene(Engine::Scene &s)
    {
        if (currentScene != nullptr)
        {
            currentScene->clean();
            currentScene->freeObjects();
            currentScene->objs.clear();
        }
        
        currentScene = &s;
        __pAssert(currentScene->init, "New scene doesn't have init function set.");
        __pAssert(currentScene->update, "New scene doesn't have update function set.")
        __pAssert(currentScene->clean, "New scene doesn't have clean function set.")
        currentScene->init();
    }

    Window window{800, 600, "Test", nullptr};
    GLuint glProgramID;
    
    static void printError(int error, const char *description)
    {
        fputs(description, stderr);
        fputs("\n", stderr);
    }

    _PONTILUS_SETTINGS *getArgs()
    {
        return &args;
    }

    void setDefaultShader(const char *vertPath, const char *fragPath)
    {
        Renderer::setDefaultShader(vertPath, fragPath);
    }

    const char *getDefaultShader(bool oneForVert)
    {
        return Renderer::getDefaultShader(oneForVert);
    }
    
    void init()
    {
        glfwSetErrorCallback(printError);

        __pAssert(glfwInit(), "Could not initialize GLFW instance");

        // GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        
        // Initialize the window
        window.ptr = glfwCreateWindow(window.width, window.height, window.title, NULL, NULL);
        
        __pAssert(window.ptr, "Could not initialize GLFW window.");

        glfwMakeContextCurrent(window.ptr);

        // v-sync
        glfwSwapInterval(1);

        // setup callbacks
        glfwSetWindowSizeCallback(window.ptr, [](GLFWwindow *r, int newWidth, int newHeight)
                                  {
                                      window.width = newWidth;
                                      window.height = newHeight;
                                      glViewport(0, 0, window.width, window.height);
                                      Renderer::Camera::updateProjection();
                                  });
        
        glfwSetCursorPosCallback(window.ptr, IO::mousePosCallback);
        glfwSetScrollCallback(window.ptr, IO::mouseScrollCallback);
        glfwSetMouseButtonCallback(window.ptr, IO::mouseButtonCallback);

        glfwSetKeyCallback(window.ptr, IO::keyPressedCallback);

        // make the window visible
        glfwShowWindow(window.ptr);

        // initialize opengl backend
        if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress))
        {
            printf("Failed to initialize OpenGL context\n");
            return;
        }

        glViewport(0, 0, window.width, window.height);
        // transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Renderer::start();
        Audio::initAudio();

        setCurrentScene(defaultScene);

        // say hi
        __pMessage("Hello: %s\n", glGetString(GL_VERSION));
    }

    void loop()
    {
        // might use these sometime
        double t1;
        double t2;
        double dt = 0.0f;
        double highestdt = 0.016f;
        double lowestdt = 0.016f;
        while (!glfwWindowShouldClose(window.ptr))
        {
            t1 = glfwGetTime();

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            // set default background
            glClear(GL_COLOR_BUFFER_BIT);

            // poll events
            glfwPollEvents();

            // update engines
            //TODO: fixed update for updateAll() and getCurrentScene()->update()
            getCurrentScene()->updateObjects(dt);
            getCurrentScene()->update(dt);
            // Physics2D::fixedUpdate();

            // render
            Renderer::render();
            Renderer::modelRender();
            Renderer::postRender();

            // swap buffers (makes things smoother)
            glfwSwapBuffers(window.ptr);

            // framerate calculations
            t2 = glfwGetTime();
            dt = t2 - t1;
            highestdt = highestdt > dt ? highestdt : dt;
            lowestdt = lowestdt < dt ? lowestdt : dt;

            IO::endFrame();
        }

        if (getCurrentScene()) {
            getCurrentScene()->clean();
            getCurrentScene()->freeObjects();
        }
            
        Renderer::close();
        Audio::closeAudio();

        glfwDestroyWindow(window.ptr);
        glLinkProgram(0);
        glfwTerminate();
    }
}

