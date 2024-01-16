/* date = January 3rd 2022 2:35 pm */

#pragma once

#include "ecs/Component.h"
#include "renderer/rData.h"

namespace Pontilus
{
    namespace Renderer
    {
        class SpriteRenderer : public ECS::Component, public Renderer::Renderable
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
            int toRData(Renderer::rData &r);

            virtual int update(double);
        };
    }
}
