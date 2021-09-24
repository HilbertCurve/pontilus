/* date = September 17th 2021 10:29 am */

#ifndef _PONTILUS_INPUT_LISTENER_H
#define _PONTILUS_INPUT_LISTENER_H

#include <GLFW/glfw3.h>

#define NUM_MOUSE_BUTTONS 6
#define NUM_KEYS 350
#define LEFTOVER_MEM 475

namespace Pontilus
{
    namespace IO
    {
        // TODO: phase out the singleton
        
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
        };

        /**
         * Checks to see if a certain mouse button is pressed.
         */
        bool isButtonPressed(int button);
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
    }
}

#endif