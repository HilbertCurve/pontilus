/* date = August 22nd 2021 10:47 am */

#ifndef _PONTILUS_APPLICATION_H
#define _PONTILUS_APPLICATION_H

#include <GLFW/glfw3.h>

#include "rData.h"
#include "Scene.h"

namespace Pontilus
{
    typedef short _PONTILUS_SETTINGS;

    extern Graphics::rData quadPool;
    extern Graphics::rData fullWindowQuad;
    extern Graphics::rData pointLightPool;
    extern Graphics::Texture *texPool[8];

    extern float resolution;

    struct Window
    {
        int width;
        int height;
        const char *title;
        GLFWwindow *ptr;
        Engine::Scene *scene;
    };

    extern Window window;

    void init();
    void loop();

    _PONTILUS_SETTINGS *getArgs();
    bool debugMode();
}

#endif //_PONTILUS_APPLICATION_H
