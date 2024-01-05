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
                SpriteRenderer() { this->init({nullptr}, glm::vec4(1.0, 1.0, 1.0, 1.0)); }
                SpriteRenderer(Renderer::Texture t, glm::vec4 color) { this->init(t, color); }
                SpriteRenderer(glm::vec4 color) { this->init({nullptr}, color); }
                void init(Renderer::Texture t, glm::vec4 color);

                Renderer::Texture tex = {nullptr, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
                glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
                bool visible = true;
                void setVisible(bool v);
                int toRData(Renderer::rData &r, unsigned int rOffset);
                void toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property);

                virtual int update(double dt) { return 0; }
            };
        }
    }
}
