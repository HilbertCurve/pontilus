/* date = October 30th 2021 3:16 pm */

#pragma once

#include <vector>

#include "GameObject.h"

namespace Pontilus
{
    namespace Engine
    {
        typedef void (* _init)();
        typedef void (* _update)(double dt);

        struct Scene
        {
            _init init;
            _update update; 

            std::vector<GameObject> objs;
        };

        void init();
        void update(double dt);

        Scene *getScene();
    }
}