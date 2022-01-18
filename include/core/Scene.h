/* date = October 30th 2021 3:16 pm */

#pragma once

#include <vector>
#include "ecs/GameObject.h"
#include "graphics/rData.h"

namespace Pontilus
{
    namespace Engine
    {
        typedef void (* _init)();
        typedef void (* _update)(double dt);
        typedef void (* _clean)();

        struct Scene
        {
            _init init;
            _update update;
            _clean clean;

            void toRData(Graphics::rData &r, unsigned int rOffset);

            std::vector<ECS::GameObject> objs = std::vector<ECS::GameObject>(0);

            int numQuads = 0;
        };

        namespace Scenes
        {
            extern Scene animation;
            extern Scene pog;
            extern Scene debug;
        }
    }
}