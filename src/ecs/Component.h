/* date = January 2nd 2022 6:56 pm */

#pragma once

#include "ecs/GameObject.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            class GameObject;

            struct Component
            {
                GameObject *parent = nullptr;

                virtual bool operator==(Component &c);
                virtual int update(double dt) = 0;
                virtual void clear() {};
            };
        }
    }
}
