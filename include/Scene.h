/* date = October 30th 2021 3:16 pm */

#pragma once
#include "Application.h"

namespace Pontilus
{
    namespace Engine
    {
        typedef void (* _init)();
        typedef void (* _update)(double dt);

        struct Scene
        {
            Pontilus::Window *parent;
            _init init;
            _update update; 
        };
    }
}