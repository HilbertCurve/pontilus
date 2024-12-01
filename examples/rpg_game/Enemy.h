/* date = November 30, 2024 3:07 PM */


#ifndef ENEMY_H
#define ENEMY_H
#include "ecs/EntityBuilder.h"

namespace RpgGame {

class Enemy : public Pontilus::ECS::EntityBuilder {
public:
    void build(Pontilus::ECS::Entity *obj) override;
};

} // RpgGame

#endif //ENEMY_H
