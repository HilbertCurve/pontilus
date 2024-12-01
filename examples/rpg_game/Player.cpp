/* date = November 30, 2024 2:19 PM */


#include "Player.h"

#include "core/InputListener.h"
#include "ecs/Transform.h"
#include "renderer/SpriteRenderer.h"

namespace RpgGame {
    int Player::Controller::update(double dt) {
        using namespace Pontilus;
        auto velocity = glm::vec3(0.0);
        if (IO::isKeyPressed(GLFW_KEY_W)) {
            velocity += glm::vec3{0.0, 1.0, 0.0};
        }
        if (IO::isKeyPressed(GLFW_KEY_S)) {
            velocity += glm::vec3{0.0, -1.0, 0.0};
        }
        if (IO::isKeyPressed(GLFW_KEY_D)) {
            velocity += glm::vec3{1.0, 0.0, 0.0};
        }
        if (IO::isKeyPressed(GLFW_KEY_A)) {
            velocity += glm::vec3{-1.0, 0.0, 0.0};
        }

        auto transform = this->parent->getComponent<ECS::Transform>();
        transform->pos += velocity;

        return 0;
    }

    void Player::build(Pontilus::ECS::Entity *obj) {
        using namespace Pontilus;
        obj->addComponent(new ECS::Transform(
            glm::vec3{1.0, 1.0, -1.0},
            glm::vec3{50.0, 50.0, 1.0},
            glm::vec3{0.0, 0.0, 0.0}
        ));
        obj->addComponent(new Renderer::SpriteRenderer({1.0, 1.0, 1.0, 1.0}));
        obj->addComponent(new Controller());
    }
} // RpgGame