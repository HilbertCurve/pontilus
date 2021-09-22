#include <stdio.h>
#include <GLFW/glfw3.h>

#include "InputListener.h"

namespace Pontilus
{
    namespace IO
    {
        //***********************************************
        // MOUSE
        //***********************************************
        Mouse &Mouse::get()
        {
            static Mouse mouse = Mouse();
            return mouse;
        }

        void Mouse::mousePosCallback(GLFWwindow *window, double xPos, double yPos)
        {
            Mouse::get().lastX = Mouse::get().xPos;
            Mouse::get().lastY = Mouse::get().yPos;
            Mouse::get().xPos = xPos;
            Mouse::get().yPos = yPos;
        }

        void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                if (button < NUM_MOUSE_BUTTONS)
                {
                    Mouse::get().buttonsPressed[button] = true;
                }
            }
            else if (action == GLFW_RELEASE)
            {
                if (button < NUM_MOUSE_BUTTONS)
                {
                    Mouse::get().buttonsPressed[button] = false;
                }
                Mouse::get().isDragging = false;
            }
        }

        void Mouse::mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
        {
            Mouse::get().scrollX = xOffset;
            Mouse::get().scrollY = yOffset;
        }

        void Mouse::endFrame()
        {
            Mouse::get().scrollX = 0;
            Mouse::get().scrollY = 0;
            Mouse::get().lastX = Mouse::get().xPos;
            Mouse::get().lastY = Mouse::get().yPos;
        }

        Mouse::Mouse()
        {
            scrollX = scrollY = 0;
            xPos = yPos = lastX = lastY = 0;

            for (int i = 0; i < NUM_MOUSE_BUTTONS; i++)
                buttonsPressed[i] = false;
            isDragging = false;
        }

        //***********************************************
        // KEYBOARD
        //***********************************************
        void Keyboard::keyPressedCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                Keyboard::get().keysPressed[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                Keyboard::get().keysPressed[key] = false;
            }
        }

        Keyboard::Keyboard()
        {
            for (int i = 0; i < NUM_KEYS; i++)
                keysPressed[i] = false;
        }

        //***********************************************
        // OTHERS
        //***********************************************
        Keyboard &Keyboard::get()
        {
            static Keyboard keyboard = Keyboard();
            return keyboard;
        }

        bool isButtonPressed(int button)
        {
            if (button >= NUM_MOUSE_BUTTONS || button < 0)
            {
                fprintf(stderr, "ERROR: tried to access illegal mouse button: %d", button);
                return false;
            }
            return Mouse::get().buttonsPressed[button];
        }

        bool isKeyPressed(int key)
        {
            if (key >= NUM_KEYS || key < 0)
            {
                fprintf(stderr, "ERROR: tried to access illegal key: %d", key);
                return false;
            }
            return Keyboard::get().keysPressed[key];
        }
    }
}