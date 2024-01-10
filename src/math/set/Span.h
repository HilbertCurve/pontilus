#pragma once

namespace Pontilus
{
    namespace Math
    {
        /**
         * A single range between two floating-point real numbers, with optional
         * inclusivity at each end.
        */
        class Span
        {
            /**
             * Returns an empty span.
             */
            Span() : _start(0.0), _end(0.0), _start_closed(false), _end_closed(false) { }
            /**
             * Returns an inclusive span from start to end.
             */
            Span(float start, float end) : _start(start), _end(end), _start_closed(true), _end_closed(true) { }
            /**
             * Returns a span with inclusivity specified.
             */
            Span(float start, float end, float sc, float ec) : _start(start), _end(end), _start_closed(sc), _end_closed(ec) { }
            /**
             * Returns the intersection between two spans, or the empty span if there is no intersection.
             */
            Span intersect(const Span &other);
            /**
             * Returns the length of the `Span`, disregarding openness.
             * 
             * Prints a warning for each end of the Span that is open. But then again, infinitesimals
             * aren't really within finite precision, so it may not matter too much.
             */
            float length();
            private:
            float _start, _end;
            bool _start_closed, _end_closed;

            static const Span EMPTY_SPAN;
        };
    }
}