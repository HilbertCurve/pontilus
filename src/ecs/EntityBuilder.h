/* date = November 30, 2024 1:24 PM */


#ifndef ENTITYBUILDER_H
#define ENTITYBUILDER_H
#include "Entity.h"

namespace Pontilus {
namespace ECS {

class EntityBuilder {
public:
    virtual ~EntityBuilder() = default;
    virtual void build(Entity *obj) = 0;
};

} // ECS
} // Pontilus

#endif //ENTITYBUILDER_H
