/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "rData.h"
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

            Graphics::Texture tex = {nullptr, 0, 0, 0, false};
            Graphics::Primitive prim = Graphics::Primitives::QUAD;
        };

        void initGameObject(GameObject &g, glm::vec3 pos, glm::vec4 color, float width, float height);

        void gameStateToRData(GameObject &s, Graphics::rData &r, unsigned int rOffset);
        void gameStateToRData(std::vector<GameObject> gs, Graphics::rData &r, unsigned int rOffset);
        void gameStateToRData(GameObject &s, Graphics::rData &r, unsigned int rOffset, Graphics::vProp property);
    }
}