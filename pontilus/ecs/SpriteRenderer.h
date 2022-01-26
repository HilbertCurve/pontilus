/* date = January 3rd 2022 2:35 pm */

#pragma once

#include "ecs/Component.h"
#include "graphics/rData.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            class SpriteRenderer : public Component, public Graphics::Renderable
            {
                public:
                SpriteRenderer();

                Graphics::Texture tex = {nullptr};
                int toRData(Graphics::rData &r, unsigned int rOffset);
                void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);
            };
        }
    }
}
