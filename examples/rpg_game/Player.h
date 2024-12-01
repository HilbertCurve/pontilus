/* date = November 30, 2024 2:19 PM */


#ifndef PLAYER_H
#define PLAYER_H
#include "ecs/EntityBuilder.h"

namespace RpgGame {

class Player : public Pontilus::ECS::EntityBuilder {
    class Controller : public Pontilus::ECS::Component {
    public:
        int update(double dt) override;
    };
public:
    void build(Pontilus::ECS::Entity *obj) override;
};

} // RpgGame

#endif //PLAYER_H
