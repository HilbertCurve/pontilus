#pragma once

#include "math.h"
#include "glm/glm.hpp"

namespace Pontilus
{
    namespace Math
    {
        // a line defined in slope-intercept form y = ax + b.
        class Line
        {
            public:
            Line() = default;
            Line(glm::vec2 p1, glm::vec2 v2);
            float a, b;
        };

        int signOrZero(float f);
        int sign(float f);

        glm::vec2 rotate(glm::vec2 v, glm::vec2 pivot, float theta);
        glm::vec2 rotate(glm::vec2 v, float theta);

        float dist(glm::vec2 v1, glm::vec2 v2);
        float distSquared(glm::vec2 v1, glm::vec2 v2);

        bool pointAboveLine(glm::vec2 p, Line &l);
    }
}