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

    class Scene
    {

        public:
        Scene() = default;
        Scene(_init i, _update u, _clean c) : init(i), update(u), clean(c) { this->used = false; };
        // SHOULD SCENE HAVE THESE????????
        // probably not, should be inheritance. I'm fighting demons
        _init init;
        _update update;
        _clean clean;
        ECS::GameObject &spawn();
        ECS::GameObject &get(int id);
        void despawn(int id);
        // DEPRECATED: use spawns
        // void addObject(ECS::GameObject obj);
        // void removeObject(int id);

        void updateObjects(double dt);
        void freeObjects();

        bool isUsed() { return this->used; }
        void setUsed(bool _used) { this->used = _used; }

        private:
        std::vector<ECS::GameObject *> objs = std::vector<ECS::GameObject *>();
        bool used = false;
    };
}

