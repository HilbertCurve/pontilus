/* date = January 05, 2025 11:40 PM */


#ifndef UIFRAME_H
#define UIFRAME_H
#include <pontilus/math/geometry/Rect.h>

namespace Pontilus {
    namespace GUI {
        /**
         * An interface for "framing" UI elements, or giving them the next available spot for them to be rendered.
         */
        class UIFrame {
        public:
            virtual Math::Rect nextFrame() = 0;

            virtual Math::Rect getFrame(size_t idx) = 0;
        };
    } // GUI
} // Pontilus

#endif //UIFRAME_H
