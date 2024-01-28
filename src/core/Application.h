/* date = August 22nd 2021 10:47 am */

#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "core/Scene.h"
#include "renderer/rData.h"
#include "renderer/Font.h"
#include "renderer/Camera.h"

namespace Pontilus
{
    class Window
    {
        private:
        static size_t _id;
        size_t id;
        size_t width;
        size_t height;
        std::string title;
        GLFWwindow *ptr;
        Scene *scene;
        bool isClosing;
        std::vector<size_t> renderTargetIDs;
        Renderer::Camera camera;
        
        public:
        Window();
        Window(Window &) = delete;

        size_t getWidth() { return width; }
        size_t getHeight() { return height; }

        void setWidth(size_t w) { this->width = w; }
        void setHeight(size_t h) { this->height = h; }

        size_t getID() { return this->id; }

        void setTitle(std::string &t);
        void update(double dt);

        Scene &getScene() { return *this->scene; }
        void setScene(Scene &s);

        Renderer::Camera &getCamera() { return this->camera; }

        bool closing() { return isClosing; }

        // Const because glfw access should be either private or immutable
        const GLFWwindow *getGLFW() { return this->ptr; }

        ~Window();
    };

    class Application
    {
        public:
        static Application *get();
        Application(Application &) = delete;
        /**
         * Initializes the Pontilus Application.
         */
        Window &getMainWindow() { return this->getWindow(0); }
        Window &getWindow(size_t id);
        Window &getWindowByGLFW(GLFWwindow *ptr);
        size_t newWindow();
        /**
         * Hands control flow of the program to the Pontilus Application.
         */
        void run();
        double fps() { return 1.0 / dt; }

        private:
        void init();
        void removeWindow(size_t id);
        std::vector<Window *> windows;
        bool printDebug;
        double dt;

        Application();
        static Application *inst;
    };
}
