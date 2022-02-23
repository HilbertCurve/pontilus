/* date = November 29th 2021 10:34 am */

#pragma once

#include "vector"

#include "physics2d/CollisionDetection.h"
#include "physics2d/Constraint.h"

namespace Pontilus
{
    namespace Physics2D
    {
        void freeResolve(pData data);
        void constraintResolve(Constraint &cst);
    }
}