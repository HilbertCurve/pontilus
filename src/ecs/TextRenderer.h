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
            class TextRenderer : public Component, public Renderer::Renderable
            {
                public:
                TextRenderer() = default;
                void init(const char *text, Renderer::Font &f);
                Renderer::Font *font;
                std::string text;

                int toRData(Renderer::rData &r, unsigned int rOffset);
                void toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property);
            };
        }
    }
}
