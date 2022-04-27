/* date = April 24th 2022 1:54 pm */

#pragma once

#include "ecs/Component.h"
#include "graphics/rData.h"
#include <nlohmann/json.hpp>

namespace Pontilus
{
    namespace Model
    {
        class ModelRenderer : public Engine::ECS::Component, public Renderer::Renderable
        {
            public:
            int toRData(Renderer::rData &r, unsigned int rOffset);
            void toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property);

            nlohmann::json gltfData;
            Renderer::rData vertices;

            void init(const char *gltfFile);
        };
    }
}

