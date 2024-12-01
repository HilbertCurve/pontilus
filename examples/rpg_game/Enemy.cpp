/* date = November 30, 2024 3:07 PM */


#include "Enemy.h"

#include "ecs/Transform.h"
#include "renderer/SpriteRenderer.h"

namespace RpgGame {
    void Enemy::build(Pontilus::ECS::Entity *obj) {
        using namespace Pontilus;
        obj->addComponent(new ECS::Transform(
            glm::vec3{1.0, 1.0, -1.0},
            glm::vec3{50.0, 50.0, 1.0},
            glm::vec3{0.0, 0.0, 0.0}
        ));
        obj->addComponent(new Renderer::SpriteRenderer({1.0, 1.0, 1.0, 1.0}));
    }
} // RpgGame