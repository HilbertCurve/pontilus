/* date = August 22nd 2021 10:47 am */

#pragma once

#include <GLFW/glfw3.h>

#include "graphics/rData.h"
#include "graphics/Font.h"
#include "ui/Scene.h"

namespace Pontilus
{
    typedef short _PONTILUS_SETTINGS;

    extern Graphics::rData quadPool;
    extern Graphics::rData linePool;
    extern Graphics::rData fullWindowQuad;
    extern Graphics::rData pointLightPool;
    extern Graphics::IconMap *iconPool[8];
    extern Graphics::Font *fontPool[8];
    extern int iconPoolStackPointer;
    extern int fontPoolStackPointer;

    extern float resolution;

    struct Window
    {
        int width;
        int height;
        const char *title;
        GLFWwindow *ptr;
    };

    extern Window window;
    
    UI::Scene *getCurrentScene();
    void setCurrentScene(UI::Scene &s);

    void init();
    void loop();

    _PONTILUS_SETTINGS *getArgs();
    bool debugMode();
}
