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
            class SpriteRenderer : public Component, public Renderer::Renderable
            {
                public:
                void init(Renderer::Texture t);

                Renderer::Texture tex = {nullptr};
                glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
                int toRData(Renderer::rData &r, unsigned int rOffset);
                void toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property);
            };
        }
    }
}
