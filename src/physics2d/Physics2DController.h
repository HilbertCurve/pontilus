/* date = November 24th 2021 2:40 pm */

#pragma once

#include <vector>

#include "ecs/Body2D.h"

namespace Pontilus
{
    namespace Physics2D
    {
        void update(double dt);
        void addBody(Engine::ECS::Body2D *b);
        void fixedUpdate();
    }
}