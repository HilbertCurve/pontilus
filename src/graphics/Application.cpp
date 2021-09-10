#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "Application.h"
#include "Renderer.h"

namespace Application
{
    struct Window
    {
        int width;
        int height;
        const char* title;
        GLFWwindow* ptr;
        bool open = false;
    };
    
    Window window {800, 600, "Test", nullptr};
    GLuint glProgramID;
    
    static void printError(int error, const char* description) 
    {
        fputs(description, stderr);
        fputs("\n", stderr);
    }

    void init() 
    {
        glfwSetErrorCallback(printError);

        if(!glfwInit()) 
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
        //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        
        // Initialize the window
        window.ptr = glfwCreateWindow(100, 100, "EEE", glfwGetPrimaryMonitor(), NULL);
        
        if (!window.ptr) 
        {
            printf("ERROR: COULD NOT INITIALIZE WINDOW.\n");
            exit(-1);
        }
        
        glfwMakeContextCurrent(window.ptr);
        
        // v-sync
        glfwSwapInterval(1);
        
        // setup callbacks
        glfwSetWindowSizeCallback(window.ptr, [](GLFWwindow* w, int newWidth, int newHeight) {
                                      glViewport(0, 0, newWidth, newHeight);
                                      window.width = newWidth;
                                      window.height = newHeight;
                                  });
        
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
        while (!glfwWindowShouldClose(window.ptr)) {
            // set default background
            glClearColor(0.01f, 0.01f, 0.01f, 0.01f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            // render
            Renderer::render();
            // swap buffers (makes things smoother)
            glfwSwapBuffers(window.ptr);
            // poll events
            glfwPollEvents();
        }
        
        glLinkProgram(0);
        //TODO(HilbertCurve): other garbage collection things
    }
}