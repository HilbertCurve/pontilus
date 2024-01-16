/* date = August 22nd 2021 10:47 am */

#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "core/Scene.h"
#include "renderer/rData.h"
#include "renderer/Font.h"

namespace Pontilus
{
    typedef short _PONTILUS_SETTINGS;

    extern float resolution;

    struct Window
    {
        int width;
        int height;
        const char *title;
        GLFWwindow *ptr;
    };

    extern Window window;
    
    // TODO: re-export stuff to make this header the public API
    Scene *getCurrentScene();
    void setCurrentScene(Scene &s);

    void setDefaultShader(const char *vertPath, const char *fragPath);
    const char *getDefaultShader(bool oneForVert);

    void init();
    void loop();

    _PONTILUS_SETTINGS *getArgs();
}
