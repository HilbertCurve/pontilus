/* date = April 24th 2022 1:54 pm */

#pragma once

#include "ecs/Component.h"
#include "graphics/rData.h"
#include <nlohmann/json.hpp>

namespace Pontilus
{
    namespace Model
    {
        class ModelRenderer : public Engine::ECS::Component, public Graphics::Renderable
        {
            public:
            int toRData(Graphics::rData &r, unsigned int rOffset);
            void toRData(Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);

            nlohmann::json gltfData;
            rData vertices;

            void init(const char *gltfFile);
        };
    }
}
