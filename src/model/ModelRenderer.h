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
            Renderer::rData *vertices = nullptr;

            void init(const char *gltfFile);

            private:
            // cache data, for toRData
            const char *gltfFP;
            std::string binFP;
            int posLength, posOffset, posType, posCount;
            int colorLength, colorOffset, colorType, colorCount;
            int indexLength, indexOffset, indexType, indexCount;
        };
    }
}

