/* date = January 3rd 2022 2:35 pm */

#pragma once

#include "ecs/Component.h"
#include "renderer/rData.h"
#include "renderer/Texture.h"

namespace Pontilus
{
    namespace Renderer
    {
        class SpriteRenderer : public ECS::Component, public Renderable
        {
            public:
            SpriteRenderer() { this->init(IconMap::emptyTexture(), glm::vec4(1.0, 1.0, 1.0, 1.0)); }
            SpriteRenderer(Renderer::Texture t, glm::vec4 c) { this->init(t, c); }
            explicit SpriteRenderer(glm::vec4 c) { this->init(IconMap::emptyTexture(), c); }
            void init(Renderer::Texture t, glm::vec4 color);
            ~SpriteRenderer() override = default;

            Renderer::Texture tex{};
            glm::vec4 color{};
            bool visible = true;
            void setVisible(bool v);

            int toRData(Renderer::rData &r) override;
            int update(double) override;
        };
    }
}
