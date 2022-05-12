/* date = January 5th 2022 8:42 am */

#pragma once

#include <string>

#include "ecs/Component.h"
#include "graphics/rData.h"
#include "graphics/Font.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            enum TextMode
            {
                CENTER_LEFT = 0,
                CENTER_MIDDLE,
                CENTER_RIGHT,
                CENTER_JUSTIFY
            };
            class TextRenderer : public Component, public Renderer::Renderable
            {
                public:
                TextRenderer() = default;
                void init(const char *text, Renderer::Font &f, glm::vec4 color);
                Renderer::Font *font;
                glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
                std::string text;
                TextMode mode = CENTER_LEFT;

                int toRData(Renderer::rData &r, unsigned int rOffset);
                void toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property);
            };
        }
    }
}
