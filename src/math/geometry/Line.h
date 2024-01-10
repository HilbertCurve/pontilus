#pragma once

#include <glm/glm.hpp>

namespace Pontilus
{
    namespace Math
    {
        // a line defined in slope-intercept form y = mx + b.
        class Line
        {
            public:
            Line() = default;
            Line(glm::vec2 p1, glm::vec2 p2);
            private:
            // slope-intercept form, in abstract Cartesian plane.
            float m, b;
        };
        class LineSegment
        {
            public:
            LineSegment() = default;
            LineSegment(glm::vec2 p1, glm::vec2 p2) : a1(p1.x), a2(p1.y), b1(p2.x), b2(p2.y) { }
            private:
            // defined as two points
            float a1,a2,b1,b2;
        };
    }
}