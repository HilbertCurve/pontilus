/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "Rend.h"
#include "Primitive.h"

namespace Pontilus
{
    namespace Engine
    {
        // THIS IS A TEST! This is just for testing the Rend API, and is subject to change.
        struct GameObject
        {
            glm::vec3 pos;
            glm::vec4 color;
            Graphics::Primitive prim = Graphics::Primitives::QUAD;
        };

        void gameStateToRend(GameObject &s, Graphics::Rend &r);
    }
}