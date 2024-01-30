#pragma once

#include <ecs/Component.h>

namespace Platformer
{
    class Item : public Pontilus::ECS::Component
    {
        public:
        virtual int update(double);
        Item();
        // ...
    };
}