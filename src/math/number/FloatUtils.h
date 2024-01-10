#pragma once

#include <glm/fwd.hpp>

namespace Pontilus
{
    namespace Math
    {
        int signOrZero(float f);
        int sign(float f);
        bool between(float left, float f, float right);
        bool betweenStrict(float left, float f, float right);
        bool between(glm::vec2 min, glm::vec2 point, glm::vec2 max);
        /**
         * Returns the length of the intersecting regions start1<->end1 and start2<->end2.
         */
        float overlap(float start1, float end1, float start2, float end2);
    }
}