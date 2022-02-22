#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/gl.h>
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
#include "physics2d/Physics2DController.h"

namespace Pontilus
{
    static _PONTILUS_SETTINGS args = 0x0000;

    float resolution = 512;

    // quad pool:
    Graphics::rData quadPool = {};

    // debug line pool:
    Graphics::rData linePool = {};
    Graphics::vAttrib linePoolAttribs[2] = 
    {
        { Graphics::PONT_POS, Graphics::PONT_FLOAT, 3 },
        { Graphics::PONT_COLOR, Graphics::PONT_FLOAT, 4 }
    };

    // fullScreenQuad
    Graphics::rData fullWindowQuad = {};
    Graphics::vAttrib fullWindowQuadAttribs[2] = 
    {
        { Graphics::PONT_POS, Graphics::PONT_FLOAT, 3 },
        { Graphics::PONT_COLOR, Graphics::PONT_FLOAT, 4 }
    };

    static void initQuads()
    {
        Graphics::initRData(quadPool, 2000);

        Graphics::initRData(fullWindowQuad, 4, fullWindowQuadAttribs, 2);
        glm::vec3 orientation;

        for (int i = 0; i < 4; i++)
        {
            float r = Renderer::Camera::projectionWidth/2;
            float u = Renderer::Camera::projectionHeight/2;
            switch (i)
            {
                case 0: orientation = { r,  u, 0.0f}; break;
                case 1: orientation = {-r,  u, 0.0f}; break;
                case 2: orientation = {-r, -u, 0.0f}; break;
                case 3: orientation = { r, -u, 0.0f}; break;
            }
            for (int j = 0; j < 3; j++)
            {
                ((float *)fullWindowQuad.data)[i * 7 + j] = orientation[j];
            }

            for (int j = 0; j < 4; j++)
            {
                ((float *)fullWindowQuad.data)[i * 7 + j + 3] = 0.00f;
            }
        }
    }

    static void initLines()
    {
        Graphics::initRData(linePool, 1000, linePoolAttribs, 2);
    }

    static void cleanQuads()
    {
        free(quadPool.data);
        free(quadPool.layout);
    }

    static void cleanLines()
    {
        free(linePool.data);
        free(linePool.layout);
    }

    // pointLight pool
    Graphics::rData pointLightPool = {};
    static Graphics::vAttrib pointLightAttributes[3] = 
    {
        { Graphics::PONT_POS,   Graphics::PONT_FLOAT, 3 },
        { Graphics::PONT_COLOR, Graphics::PONT_FLOAT, 4 },
        { Graphics::PONT_OTHER, Graphics::PONT_FLOAT, 1 }
    };

    static void initPointLights()
    {
        Graphics::initRData(pointLightPool, 16, pointLightAttributes, 3);
    }

    static void cleanPointLights()
    {
        free(pointLightPool.data);
        free(pointLightPool.layout);
    }

    // Texture pool:
    Graphics::IconMap *iconPool[8];
    int iconPoolStackPointer = 0;

    static void initTexPool()
    {
        for (int i = 0; i < 8; i++)
        {
            iconPool[i] = nullptr;
        }
    }

    static void cleanTexPool()
    {
        for (int i = 0; i < 8; i++)
        {
            iconPool[i] = nullptr;
        }
    }

    Graphics::Font *fontPool[8];
    int fontPoolStackPointer = 0;

    static void initFontPool()
    {
        for (int i = 0; i < 8; i++)
        {
            fontPool[i] = nullptr;
        }
    }

    // i'd prefer to keep this private; there are some quirks with getting and setting this variable i'd rather automate
    static Engine::Scene *currentScene;

    Engine::Scene *getCurrentScene()
    {
        return currentScene;
    }

    void setCurrentScene(Engine::Scene &s)
    {
        if (currentScene != nullptr)
            currentScene->clean();
        
        currentScene = &s;
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
    
    void init()
    {
        // init memory pools
        initQuads();
        initPointLights();

        glfwSetErrorCallback(printError);
        
        if (!glfwInit())
        {
            printf("ERROR: COULD NOT INITIALIZE GLFW.\n");
            exit(-1);
        }
        
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
        
        if (!window.ptr)
        {
            fprintf(stderr, "ERROR: COULD NOT INITIALIZE WINDOW.\n");
            exit(-1);
        }
        
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
        
        glViewport(0, 0, window.width, window.height);
        // transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Renderer::start();
        Audio::initAudio();
        
        setCurrentScene(Engine::Scenes::defaultScene);
        
        // say hi
        printf("Hello: %s\n", glGetString(GL_VERSION));
    }
    
    void loop()
    {
        double t1;
        double t2;
        double dt = 0.0f;
        double timeAccum = 0.0f;
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

            if (timeAccum >= 0.016f)
            {
                timeAccum = 0;
            }
            /*
            if (IO::isKeyPressed(GLFW_KEY_W))
            {
                Renderer::Camera::move(0, 0, 0.01);
            }
            if (IO::isKeyPressed(GLFW_KEY_S))
            {
                Renderer::Camera::move(0, 0, -0.01);
            }
            if (IO::isKeyPressed(GLFW_KEY_A))
            {
                Renderer::Camera::move(0.01, 0, 0);
            }
            if (IO::isKeyPressed(GLFW_KEY_D))
            {
                Renderer::Camera::move(-0.01, 0, 0);
            }
            if (IO::isKeyPressed(GLFW_KEY_R))
            {
                highestdt = lowestdt = dt;
            }
            
            glm::vec2 dMousePos = IO::mousePosChange();
            
            Renderer::Camera::rotate(dMousePos.y/50.0f, dMousePos.x/50.0f);
            */
            static bool keyIsPressed0 = false;
            static bool keyIsPressed1 = false;
            if (IO::isKeyPressed(GLFW_KEY_LEFT_SHIFT) && IO::isKeyPressed(GLFW_KEY_LEFT_CONTROL) && IO::isKeyPressed(GLFW_KEY_R))
            {
                keyIsPressed0 = true;
                if (!(keyIsPressed0 == keyIsPressed1))
                {
                    Graphics::printRData(quadPool, getCurrentScene()->numQuads * 4);
                    // don't do this; inputs and game logic should be handled in the scene, not in this loop
                    keyIsPressed1 = keyIsPressed0 = true;
                }
            }
            else
            {
                keyIsPressed1 = false;
            }

            Audio::updateListener();
            Audio::updateSources();
            Engine::ECS::StateMachine::updateAll(dt);
            getCurrentScene()->update(dt);
            Physics2D::fixedUpdate();
           
            // render
            Renderer::render();
            //Renderer::postRender();
            
            // swap buffers (makes things smoother)
            glfwSwapBuffers(window.ptr);
            
            
            // framerate calculations
            t2 = glfwGetTime();
            dt = t2 - t1;
            highestdt = highestdt > dt ? highestdt : dt;
            lowestdt = lowestdt < dt ? lowestdt : dt;
            
            // for some reason, this crashes my vm at school
            //glfwSetWindowTitle(window.ptr, window.title);
            
            IO::endFrame();

            timeAccum += dt;
        }

        cleanQuads();
        cleanPointLights();
        cleanTexPool();
        
        //Audio::closeAudio();

        glfwDestroyWindow(window.ptr);
        glLinkProgram(0);
        glfwTerminate();
    }
}
