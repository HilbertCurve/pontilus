/* date = January 2nd 2022 6:56 pm */

#pragma once

#include "ecs/Entity.h"

namespace Pontilus
{
    namespace ECS
    {
        class Entity;

        class Component
        {

        public:
            virtual bool operator==(Component &c);
            virtual int update(double dt) = 0;
            virtual void clear() {};
            virtual bool isSingleton() { return false; }
            virtual ~Component() = default;
            Entity *parent = nullptr;
        };
    }
}
