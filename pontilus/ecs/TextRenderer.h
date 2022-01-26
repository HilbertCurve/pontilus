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
            class TextRenderer : public Component, public Graphics::Renderable
            {
                public:
                TextRenderer() = default;
                void init(const char *text, Graphics::Font &f);
                Graphics::Font *font;
                std::string text;

                int toRData(Graphics::rData &r, unsigned int rOffset);
                void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);
            };
        }
    }
}
