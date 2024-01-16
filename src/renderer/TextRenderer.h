/* date = January 5th 2022 8:42 am */

#pragma once

#include <string>

#include "ecs/Component.h"
#include "renderer/rData.h"
#include "renderer/Font.h"

namespace Pontilus
{
    namespace Renderer
    {
        enum TextMode
        {
            CENTER_LEFT = 0,
            CENTER_MIDDLE,
            CENTER_RIGHT,
            CENTER_JUSTIFY
        };
        class TextRenderer : public ECS::Component, public Renderer::Renderable
        {
            public:
            TextRenderer(const char *text, Renderer::Font &f, glm::vec4 color)
            {
                this->init(text, f, color);
            }
            void init(const char *text, Renderer::Font &f, glm::vec4 color);
            Renderer::Font *font;
            glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
            std::string text;
            TextMode mode = CENTER_LEFT;

            int toRData(Renderer::rData &r);

            virtual int update(double _dt) { return 0; }
        };
    }
}
