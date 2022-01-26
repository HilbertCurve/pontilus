/* date = September 17th 2021 10:29 am */

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define NUM_MOUSE_BUTTONS 6
#define NUM_KEYS 350
#define LEFTOVER_MEM 475
#ifndef MAX_CALLBACKS
# define MAX_CALLBACKS 64
#endif

namespace Pontilus
{
    namespace IO
    {
        struct Mouse
        {
            // 199 bits
            float scrollX, scrollY;
            float xPos, yPos, lastX, lastY;
            bool buttonsPressed[NUM_MOUSE_BUTTONS];
            bool isDragging;
        };

        struct Keyboard
        {
            // 350 bits
            bool keysPressed[NUM_KEYS];
        };

        struct _buttonCallback
        {
            int id;
            void (* ptr)(int button, int action);
        };

        struct _scrollCallback
        {
            int id;
            void (* ptr)(float dx, float dy);
        };

        struct _keyCallback
        {
            int id;
            void (* ptr)(int key, int action);
        };

        // TOTAL MEMORY: 1 kb
        struct _IO
        {
            union {
                // 549 bits
                Mouse _m;
                Keyboard _k;
            } core;
            // do with this as you please (perhaps some other usb input method)
            bool misc[LEFTOVER_MEM];

            _IO(); // to initialize empty callbacks
        };

        /**
         * Checks to see if a certain mouse button is pressed.
         */
        bool isButtonPressed(int button);
        
        /**
         * Gets the change in position the mouse cursor took.
         */
        glm::vec2 mousePos();
        glm::vec2 mousePosChange();
        
        /**
         * Checks to see if a certain key is pressed.
         */
        bool isKeyPressed(int key);
        /**
         * Resets mouse drag.
         */
        void endFrame();

        // GLFW callbacks
        void mousePosCallback(GLFWwindow *window, double xPos, double yPos);
        void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
        void keyPressedCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

        // Application callbacks
        int submitButtonCallback(void (* callback)(int button, int action));
        int submitScrollCallback(void (* callback)(float dx, float dy));
        int submitKeyCallback(void (* callback)(int key, int action));

        void removeButtonCallback(int id);
        void removeScrollCallback(int id);
        void removeKeyCallback(int id);
    }
}
