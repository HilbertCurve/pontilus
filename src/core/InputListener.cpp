#include <stdio.h>
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/InputListener.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace IO
    {
        static _IO instance = _IO();

        //***********************************************
        // MOUSE CALLBACKS
        //***********************************************

        static _buttonCallback _bcs[MAX_CALLBACKS];
        static _scrollCallback _scs[MAX_CALLBACKS];

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
            else // bad action
            {
                return;
            }

            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_bcs[i].id == 0) continue;
                _bcs[i].ptr(button, action);
            }
        }

        void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
        {
            instance.core._m.scrollX = xOffset;
            instance.core._m.scrollY = yOffset;

            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_scs[i].id == 0) continue;
                _scs[i].ptr(xOffset, yOffset);
            }
        }

        int submitButtonCallback(void (* callback)(int button, int action))
        {
            _buttonCallback bc;
            bc.ptr = callback;

            // insert callback
            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_bcs[i].id == 0)
                {
                    bc.id = (uint64_t) &_bcs[i];
                    _bcs[i] = bc;
                    return bc.id;
                }
            }

            __pWarning("Could not submit button callback; maximum size reached!");
            return bc.id;
        }

        int submitScrollCallback(void (* callback)(float dx, float dy))
        {
            _scrollCallback sc;
            sc.ptr = callback;

            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_scs[i].id == 0)
                {
                    sc.id = (uint64_t) &_scs[i];
                    _scs[i] = sc;
                    return sc.id;
                }
            }

            __pWarning("Could not submit scroll callback; maximum size reached!");
            return sc.id;
        }

        void removeButtonCallback(int id)
        {
            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_bcs[i].id == id)
                {
                    _bcs[i] = {0, nullptr};
                    return;
                }
            }

            __pMessage("Button callback id: %d doesn't exist.", id);
        }

        void removeScrollCallback(int id)
        {
            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_scs[i].id == id)
                {
                    _scs[i] = {0, nullptr};
                    return;
                }
            }

            __pMessage("Scroll callback id: %d doesn't exist.", id);
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

        static _keyCallback _kcs[MAX_CALLBACKS];

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
            else // bad action
            {
                return;
            }

            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_kcs[i].id == 0) continue;
                _kcs[i].ptr(key, action);
            }
        }

        int submitKeyCallback(void (* callback)(int key, int action))
        {
            _keyCallback kc;
            kc.ptr = callback;

            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_kcs[i].id == 0)
                {
                    kc.id = (uint64_t) &_kcs[i];
                    _kcs[i] = kc;
                    return kc.id;
                }
            }

            __pWarning("Could not submit key callback; maximum size reached!")
            return kc.id;
        }

        void removeKeyCallback(int id)
        {
            // TODO: binary search
            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                if (_kcs[i].id == id)
                {
                    _kcs[i] = {0, nullptr};
                    return;
                }
            }

            __pMessage("Key callback id: %d doesn't exist.", id);
        }

        //***********************************************
        // OTHERS
        //***********************************************

        bool isButtonPressed(int button)
        {
            if (button >= NUM_MOUSE_BUTTONS || button < 0)
            {
                __pWarning("Tried to access illegal mouse button %d.", button);
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

        glm::vec2 mouseScroll()
        {
            return glm::vec2(instance.core._m.scrollX, instance.core._m.scrollY);
        }

        bool isKeyPressed(int key)
        {
            if (key >= NUM_KEYS || key < 0)
            {
                __pWarning("Tried to access illegal key %d.", key);
                return false;
            }
            return instance.core._k.keysPressed[key];
        }

        _IO::_IO()
        {
            for (int i = 0; i < MAX_CALLBACKS; i++)
            {
                _bcs[i] = {0, nullptr};
                _scs[i] = {0, nullptr};
                _kcs[i] = {0, nullptr};
            }
        }
    }
}
