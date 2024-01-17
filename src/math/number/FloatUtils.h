#pragma once

#include <glm/fwd.hpp>
#include <math.h>

namespace Pontilus
{
    namespace Math
    {
        int sign(float f);
        bool between(float left, float f, float right);
        bool betweenStrict(float left, float f, float right);
        bool between(glm::vec2 min, glm::vec2 point, glm::vec2 max);
    }
}