#pragma once

#include "math.h"
#include "glm/glm.hpp"

namespace Pontilus
{
    namespace Math
    {
        int signOrZero(float f);
        int sign(float f);

        glm::vec2 rotate(glm::vec2 v, glm::vec2 pivot, float theta);
        glm::vec2 rotate(glm::vec2 v, float theta);

        float dist(glm::vec2 v1, glm::vec2 v2);
        float distSquared(glm::vec2 v1, glm::vec2 v2);
    }
}