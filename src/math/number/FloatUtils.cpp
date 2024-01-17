#include "FloatUtils.h"

#include <glm/glm.hpp>

namespace Pontilus
{
    namespace Math
    {
        int sign(float f) {
            if (f > 0.0f) return 1;
            else if (f < 0.0f) return -1;
            else return 0;
        }
        bool between(float left, float f, float right) {
            if (left <= f && f <= right) return true;
            else return false;
        }
        bool betweenStrict(float l, float f, float r) {
            if (l < f && f < r) return true;
            else return false;
        }
        bool between(glm::vec2 min, glm::vec2 point, glm::vec2 max) {
            return between(min.x, point.x, max.x) && between(min.y, point.y, max.y);
        }


    }
}