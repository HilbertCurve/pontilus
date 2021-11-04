#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Application.h"
#include "Renderer.h"
#include "InputListener.h"
#include "Scene.h"
#include "Rend.h"
#include "Texture.h"

namespace Pontilus
{
    static _PONTILUS_SETTINGS args = 0x0000;

    // rData pool:
    Graphics::Rend rDataPool = {};
    static void initRData()
    {
        Graphics::initRend(rDataPool, 1000);
    }

    static void cleanRData()
    {
        free(rDataPool.data);
        free(rDataPool.layout);
    }

    // Texture pool:
    Graphics::Texture *texPool[8];
    static void initTexPool()
    {
        for (int i = 0; i < 8; i++)
        {
            texPool[i] = nullptr;
        }
    }

    static void cleanTexPool()
    {
        for (int i = 0; i < 8; i++)
        {
            texPool[i] = nullptr;
        }
    }

    Window window{800, 600, "Test", nullptr, Engine::getScene()};
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

    bool debugMode()
    {
        if ((*getArgs() & 0x0001) == 1) return true;
        else return false;
    }
    
    void init()
    {
        // init memory pools
        initRData();

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
        glfwSetWindowSizeCallback(window.ptr, [](GLFWwindow *w, int newWidth, int newHeight)
                                  {
                                      window.width = newWidth;
                                      window.height = newHeight;
                                      glViewport(
                                          (window.width  - 512) / 2, 
                                          (window.height - 512) / 2, 
                                          512, 
                                          512);
                                  });
        
        glfwSetCursorPosCallback(window.ptr, IO::mousePosCallback);
        glfwSetScrollCallback(window.ptr, IO::mouseScrollCallback);
        glfwSetMouseButtonCallback(window.ptr, IO::mouseButtonCallback);
        
        glfwSetKeyCallback(window.ptr, IO::keyPressedCallback);
        
        // make the window visible
        glfwShowWindow(window.ptr);
        
        glViewport(
            (window.width  - 512) / 2, 
            (window.height - 512) / 2, 
            512, 
            512);
        // transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        window.scene->init();

        // start renderer
        Renderer::start();
        
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
            
            glClearColor(0.01f, 0.01f, 0.01f, 0.01f);
            
            // set default background
            glClear(GL_COLOR_BUFFER_BIT);
            
            // poll events
            glfwPollEvents();

            if (timeAccum >= 0.016f)
            {
                window.scene->update(0.016f);
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
            if (IO::isKeyPressed(GLFW_KEY_R))
            {
                keyIsPressed0 = true;
                if (!(keyIsPressed0 == keyIsPressed1))
                {
                    Graphics::printRend(rDataPool, 5);
                    keyIsPressed1 = keyIsPressed0 = true;
                }
            }
            else
            {
                keyIsPressed1 = keyIsPressed1 = false;
            }
           
            // render
            Renderer::render();
            
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

        cleanRData();
        cleanTexPool();
        
        glLinkProgram(0);
        glfwDestroyWindow(window.ptr);
        glfwTerminate();
        //TODO(HilbertCurve): other garbage collection things
    }
}
