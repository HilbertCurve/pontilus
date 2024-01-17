#include "Span.h"
#include "math/number/FloatUtils.h"

namespace Pontilus
{
    namespace Math
    {
        Span Span::intersect(const Span &other) {
            // |---| = this
            // *---* = other
            if (this->_start <= other._end) {
                if (this->_start >= other._start) {
                    return Span(this->_start, fmin(this->_end, other._end));
                } else {
                    return Span(other._start, fmin(this->_end, other._end));
                }
            }

            return EMPTY_SPAN;
        }

        float Span::length() {
            return this->_end - this->_start;
        }

        const Span Span::EMPTY_SPAN = Span();
    }
}