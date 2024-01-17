#pragma once

#include <glm/glm.hpp>

namespace Pontilus
{
    namespace Math
    {
        class Rect
        {
            public:
            Rect() = default;
            Rect(glm::vec2 bottom_left, glm::vec2 top_right) : _bottom_left(bottom_left), _top_right(top_right) { }
            /**
             * Checks if two `Rect`s are intersecting, including at boundary.
             */
            Rect intersect(const Rect &other);
            glm::vec2 center();
            /**
             * Returns the position of the corner at a certain `quadrant` defined by
             * the Cartesian quadrants (with the center of this `Rect` as the origin).
             * 
             * NOTE: Valid quadrant values range from 1 to 4 inclusive. Invalid values will
             * result in a runtime error.
             */
            glm::vec2 corner(int quadrant);
            /**
             * Returns a `Rect` who's sides have been pushed inward by `bezel_width` units.
             */
            Rect bezel_in(float bezel_width);
            /**
             * Returns a `Rect` who's sides have been pushed outward by `bezel_width` units.
             */
            Rect bezel_out(float bezel_width);

            float width();
            float height();

            glm::vec2 &bottom_left() { return _bottom_left; }
            glm::vec2 &top_right() { return _top_right; }

            private:
            glm::vec2 _bottom_left;
            glm::vec2 _top_right; 
        };
    }
}