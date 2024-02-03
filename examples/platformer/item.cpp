#include "item.h"

#include <ecs/Transform.h>
#include <ecs/GameObject.h>

namespace Platformer
{
    static double GRAVITY = 10.0f;
    int Item::update(double dt) {
        using namespace Pontilus::ECS;

        this->velocity.y -= GRAVITY * dt;

        Transform *t = (Transform *) this->parent->getComponent(typeid(Transform));
        //if (this->collider) {
        //    if ()
        //}
        // update position


    }
}