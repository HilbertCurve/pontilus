#include "model/ModelRenderer.h"

#include "graphics/Renderer.h"
#include "ecs/Component.h"
#include <nlohmann/json.hpp>

namespace Pontilus
{
    namespace Model
    {
        void ModelRenderer::init(const char *gltfFile)
        {
            this->gltfData = getJSON(gltfFile);

        }

    }
}
