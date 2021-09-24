#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

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
        window.ptr = glfwCreateWindow(100, 100, "EEE", NULL, NULL);

        if (!window.ptr)
        {
            printf("ERROR: COULD NOT INITIALIZE WINDOW.\n");
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
        while (!glfwWindowShouldClose(window.ptr))
        {
            t1 = glfwGetTime();

            glClearColor(0.01f, 0.01f, 0.01f, 0.01f);

            // set default background
            glClear(GL_COLOR_BUFFER_BIT);

            if (IO::isKeyPressed(GLFW_KEY_W))
            {
                Renderer::Camera::move(0, 0, dt);
            }
            if (IO::isKeyPressed(GLFW_KEY_D))
            {
                Renderer::Camera::move(0, 0, -dt);
            }

            // render
            Renderer::render();

            // swap buffers (makes things smoother)
            glfwSwapBuffers(window.ptr);
            // poll events
            glfwPollEvents();

            t2 = glfwGetTime();
            dt = t2 - t1;
        }

        glLinkProgram(0);
        glfwTerminate();
        //TODO(HilbertCurve): other garbage collection things
    }
}