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
    };
    
    Window window {800, 600, "Test", nullptr};
    GLuint glProgramID;
    
    void init() 
    {
        if(!glfwInit()) 
        {
            printf("ERROR: COULD NOT INITIALIZE GLFW.\n");
            abort();
        }
        
        // GLFW
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        
        // Initialize the window
        window.ptr = glfwCreateWindow(window.width, window.height, window.title, nullptr, nullptr);
        
        if (window.ptr == nullptr) 
        {
            printf("ERROR: COULD NOT INITIALIZE WINDOW.\n");
            abort();
        }
        
        glfwMakeContextCurrent(window.ptr);
        
        // v-sync
        glfwSwapInterval(1);
        
        // setup callbacks
        glfwSetWindowSizeCallback(window.ptr, [](GLFWwindow* w, int newWidth, int newHeight) {
                                      glViewport(0, 0, newWidth, newHeight);
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
            Renderer::update();
            // swap buffers (makes things smoother)
            glfwSwapBuffers(window.ptr);
            // poll events
            glfwPollEvents();
        }
        
        glLinkProgram(0);
        //TODO(hilbertcurve): other garbage collaction thangs
    }
}