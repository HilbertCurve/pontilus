#include "math/geometry/Rect.h"

#include "math/set/Span.h"

namespace Pontilus
{
    namespace Math
    {
        Rect Rect::intersect(const Rect &other) {
            Span thisX = Span(this->_bottom_left.x, this->_top_right.x);
            Span thisY = Span(this->_bottom_left.y, this->_top_right.y);

            Span otherX = Span(other._bottom_left.x, other._top_right.x);
            Span otherY = Span(other._bottom_left.y, other._top_right.y);

            Span intersectX = thisX.intersect(otherX);
            Span intersectY = thisY.intersect(otherY);

            if (intersectX.length() == 0.0f || intersectY.length() == 0.0f)
                return Rect({0.0f, 0.0f}, {0.0f, 0.0f});
            
            return Rect({intersectX.start(), intersectY.start()}, {intersectX.end(), intersectY.end()});
        }

        glm::vec2 Rect::center() {
            return (_bottom_left + _top_right) / 2.0f;
        }

        float Rect::width() {
            return _top_right.x - _bottom_left.x;
        }
        float Rect::height() {
            return _top_right.y - _bottom_left.y;
        }
    } // namespace Math
    
}