#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Application.h"
#include "Renderer.h"
#include "InputListener.h"

namespace Pontilus
{
    struct Window
    {
        int width;
        int height;
        const char *title;
        GLFWwindow *ptr;
    };
    
    Window window{800, 600, "Test", nullptr};
    GLuint glProgramID;
    
    static void printError(int error, const char *description)
    {
        fputs(description, stderr);
        fputs("\n", stderr);
    }
    
    void init()
    {
        glfwSetErrorCallback(printError);
        
        if (!glfwInit())
        {
            printf("ERROR: COULD NOT INITIALIZE GLFW.\n");
            exit(-1);
        }
        
        // GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
                                      glViewport(0, 0, newWidth, newHeight);
                                      window.width = newWidth;
                                      window.height = newHeight;
                                  });
        
        glfwSetCursorPosCallback(window.ptr, IO::mousePosCallback);
        glfwSetScrollCallback(window.ptr, IO::mouseScrollCallback);
        glfwSetMouseButtonCallback(window.ptr, IO::mouseButtonCallback);
        
        glfwSetKeyCallback(window.ptr, IO::keyPressedCallback);
        
        // make the window visible
        glfwShowWindow(window.ptr);
        
        // transparency stuff
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        
        // start renderer
        Renderer::start();
        
        // say hi
        printf("Hello: %s\n", glGetString(GL_VERSION));
    }
    
    void loop()
    {
        double t1;
        double t2;
        double dt = 0.16f;
        double highestdt = 0.16f;
        double lowestdt = 0.16f;
        while (!glfwWindowShouldClose(window.ptr))
        {
            t1 = glfwGetTime();
            
            glClearColor(0.01f, 0.01f, 0.01f, 0.01f);
            
            // set default background
            glClear(GL_COLOR_BUFFER_BIT);
            
            // poll events
            glfwPollEvents();
            

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
            
            // render
            Renderer::render();
            
            // swap buffers (makes things smoother)
            glfwSwapBuffers(window.ptr);
            
            
            // framerate calculations
            t2 = glfwGetTime();
            dt = t2 - t1;
            highestdt = highestdt > dt ? highestdt : dt;
            lowestdt = lowestdt < dt ? lowestdt : dt;
            
            // display framerate
            std::string title; // std::string is so much easier than const char *. I'm sorry. 
            title += std::string(window.title) + 
                " || Fastest: " + std::to_string(1/lowestdt) +
                " Slowest: " + std::to_string(1/highestdt) + 
                " Current: " + std::to_string(1/dt);
            
            //glfwSetWindowTitle(window.ptr, title.c_str());
            
            IO::endFrame();
        }
        
        glLinkProgram(0);
        glfwDestroyWindow(window.ptr);
        glfwTerminate();
        //TODO(HilbertCurve): other garbage collection things
    }
}