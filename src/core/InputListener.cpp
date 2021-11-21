#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "InputListener.h"

namespace Pontilus
{
    namespace IO
    {
        
        static _IO instance = _IO();

        //***********************************************
        // MOUSE CALLBACKS
        //***********************************************
        
        void mousePosCallback(GLFWwindow *window, double xPos, double yPos)
        {
            instance.core._m.xPos = xPos;
            instance.core._m.yPos = yPos;
        }

        void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                if (button < NUM_MOUSE_BUTTONS)
                {
                    instance.core._m.buttonsPressed[button] = true;
                }
            }
            else if (action == GLFW_RELEASE)
            {
                if (button < NUM_MOUSE_BUTTONS)
                {
                    instance.core._m.buttonsPressed[button] = false;
                }
                instance.core._m.isDragging = false;
            }
        }

        void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
        {
            instance.core._m.scrollX = xOffset;
            instance.core._m.scrollY = yOffset;
        }

        void endFrame()
        {
            instance.core._m.lastX = instance.core._m.xPos;
            instance.core._m.lastY = instance.core._m.yPos;
            instance.core._m.scrollX = 0;
            instance.core._m.scrollY = 0;
        }

        //***********************************************
        // KEYBOARD CALLBACKS
        //***********************************************

        void keyPressedCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                instance.core._k.keysPressed[key] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                instance.core._k.keysPressed[key] = false;
            }
        }

        //***********************************************
        // OTHERS
        //***********************************************

        bool isButtonPressed(int button)
        {
            if (button >= NUM_MOUSE_BUTTONS || button < 0)
            {
                fprintf(stderr, "ERROR: tried to access illegal mouse button: %d\n", button);
                return false;
            }
            return instance.core._m.buttonsPressed[button];
        }
        
        glm::vec2 mousePos()
        {
            return glm::vec2(instance.core._m.xPos, instance.core._m.yPos);
        }
        
        glm::vec2 mousePosChange()
        {
            return glm::vec2(
                             instance.core._m.xPos - instance.core._m.lastX,
                             instance.core._m.yPos - instance.core._m.lastY
                             );
        }

        bool isKeyPressed(int key)
        {
            if (key >= NUM_KEYS || key < 0)
            {
                fprintf(stderr, "ERROR: tried to access illegal key: %d", key);
                return false;
            }
            return instance.core._k.keysPressed[key];
        }
    }
}