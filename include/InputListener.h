/* date = September 17th 2021 10:29 am */

#ifndef _PONTILUS_INPUT_LISTENER_H
#define _PONTILUS_INPUT_LISTENER_H

#include <GLFW/glfw3.h>

#define NUM_MOUSE_BUTTONS 6
#define NUM_KEYS 350

namespace Pontilus
{
    namespace IO
    {
        struct Mouse
        {
            static Mouse &get();

            double scrollX, scrollY;
            double xPos, yPos, lastX, lastY;
            bool buttonsPressed[NUM_MOUSE_BUTTONS];
            bool isDragging;

            static void mousePosCallback(GLFWwindow *window, double xPos, double yPos);
            static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
            static void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

            void endFrame();

            private:
            // no touchy
            Mouse();
        };

        struct Keyboard
        {
            static Keyboard &get();


            bool keysPressed[NUM_KEYS];

            static void keyPressedCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

            private:
            Keyboard();
        };

        Mouse &getMouse();
        Keyboard &getKeyboard();

        /**
         * Checks to see if a certain mouse button is pressed.
         */
        bool isButtonPressed(int button);
        /**
         * Checks to see if a certain key is pressed.
         */
        bool isKeyPressed(int key);
    }
}

#endif