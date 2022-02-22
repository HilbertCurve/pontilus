/* date = November 24th 2021 2:40 pm */

#pragma once

#include <vector>

#include "physics2d/Body2D.h"

namespace Pontilus
{
    namespace Physics2D
    {
        void updateBodies(double dt);
        void addBody(Body2D *b);
        void fixedUpdate();
    }
}