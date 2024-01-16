#include "ecs/Component.h"

namespace Pontilus
{
    namespace ECS
    {
        bool Component::operator==(Component &c)
        {
            return typeid(*this) == typeid(c) && this == &c;
        }
    }
}
