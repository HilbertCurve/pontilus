/* date = November 29th 2021 10:34 am */

#pragma once

#include "vector"

#include "physics2d/CollisionDetection.h"
#include "physics2d/Constraint.h"

namespace Pontilus
{
    namespace Physics2D
    {
        struct CollisionEvent
        {
            pData &data;
            glm::vec2 b1_impulse;
            glm::vec2 b2_impulse;
            float b1_moment;
            float b2_moment;
        };
        CollisionEvent nonEvent(pData &data);

        CollisionEvent freeResolve(pData data);
        void constraintResolve(Constraint &cst);
    }
}