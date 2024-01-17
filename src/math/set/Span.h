#pragma once

namespace Pontilus
{
    namespace Math
    {
        /**
         * A single range between two floating-point real numbers
        */
        class Span
        {
            public:
            /**
             * Returns an empty span.
             */
            Span() : _start(0.0), _end(0.0) { }
            /**
             * Returns a span from start to end.
             */
            Span(float start, float end) : _start(start), _end(end) { }
            /**
             * Returns the intersection between two spans, or the empty span if there is no intersection.
             */
            Span intersect(const Span &other);
            /**
             * Returns the length of the `Span`.
             */
            float length();

            float start() { return _start; }
            float end() { return _end; }

            private:
            float _start, _end;

            static const Span EMPTY_SPAN;
        };
    }
}