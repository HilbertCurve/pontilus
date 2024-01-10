#include "Span.h"
#include "math/number/FloatUtils.h"

namespace Pontilus
{
    namespace Math
    {
        Span Span::intersect(const Span &other) {
            Span ret = EMPTY_SPAN;
            if (Math::between(other._start, _start, other._end)) {
                ret = Span {_start, other._end, _start_closed, other._end_closed};
                if (other._start == _start && !other._start_closed)
                ret._start_closed = false;
            } else if (Math::between(other._start, _end, other._end)) {
                // ret = TODO 
            }

            return ret;
        }
        const Span Span::EMPTY_SPAN = Span();
    }
}