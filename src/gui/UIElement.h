#pragma once

namespace Pontilus
{
    // forward declarations
    namespace Math
    {
        class Rect;
    }

    namespace GUI
    {
        class UIElement
        {
            /**
             * Every UIElement must be renderable to a certain extent. 
            */
            virtual void render(Math::Rect &subspace) = 0;
            virtual void update() = 0;
        };
    }
}
