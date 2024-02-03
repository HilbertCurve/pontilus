#pragma once

#include "tilemap.h"

#include <ecs/Component.h>
#include <glm/vec3.hpp>

namespace Platformer
{
    // more so a marker component
    class Item : public Pontilus::ECS::Component
    {
        glm::vec3 velocity;
        TileMap *collider;

        public:
        virtual int update(double);
        Item();
        // ...
    };
}