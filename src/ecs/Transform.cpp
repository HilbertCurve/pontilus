#include "ecs/Transform.h"

namespace Pontilus
{
    namespace ECS
    {
        int Transform::update(double) { return 0; }
        bool Transform::operator==(Transform &other) {
            return (pos == other.pos) && (whd == other.whd) && (rot == other.rot);
        }
    }
}