/* date = October 30th 2021 3:16 pm */

#pragma once

#include <vector>
#include "ecs/GameObject.h"
#include "renderer/rData.h"

namespace Pontilus
{
    typedef void (* _init)();
    typedef void (* _update)(double dt);
    typedef void (* _clean)();

    namespace ECS
    {
        class GameObject;
    }

    struct Scene
    {
        // SHOULD SCENE HAVE THESE????????
        // probably not, should be inheritance. I'm fighting demons
        _init init;
        _update update;
        _clean clean;

        std::vector<ECS::GameObject *> objs = std::vector<ECS::GameObject *>();
        ECS::GameObject &spawn();
        void despawn(int id);
        // DEPRECATED: use spawns
        // void addObject(ECS::GameObject obj);
        // void removeObject(int id);

        void updateObjects(double dt);
        void freeObjects();
    };
}

