#pragma once

namespace Pontilus {
    // forward declarations
    namespace Math {
        class Rect;
    }

    namespace GUI {
        /**
         * An interface for letting an object be rendered as a UI element.
         */
        class UIElement
        {
            /**
             * Every UIElement must be renderable to a certain extent.
            */
            virtual void render() = 0;
            virtual void update() = 0;
        };
    }
}
