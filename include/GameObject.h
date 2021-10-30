/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "Rend.h"
#include "Primitive.h"
#include "Texture.h"

namespace Pontilus
{
    namespace Engine
    {
        struct GameObject
        {
            glm::vec3 pos;
            glm::vec4 color;
            float width, height;

            Graphics::Texture tex;
            Graphics::Primitive prim = Graphics::Primitives::QUAD;
        };

        void gameStateToRend(GameObject &s, Graphics::Rend &r, unsigned int rOffset);
        void gameStateToRend(std::vector<GameObject> gs, Graphics::Rend &r, unsigned int rOffset);
    }
}