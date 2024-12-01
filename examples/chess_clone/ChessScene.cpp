/* date = November 08, 2024 12:41 AM */


#include "ChessScene.h"

#include <iostream>

#include "Grid.h"
#include "core/InputListener.h"

namespace ChessClone {
    constexpr float TILE_SIZE = 8.0;
    constexpr float TILE_PADDING = 1.0;
    constexpr glm::vec4 EMPTY_TILE = glm::vec4(0.3);
    static size_t tmp_obj = 0;

    void ChessScene::init() {
        auto obj = &this->spawn();

        obj->addComponent(new Grid());

        // auto obj = &this->spawn();
        //
        // obj->addComponent(new Pontilus::Renderer::SpriteRenderer({1.0, 1.0, 1.0, 1.0}));
        // obj->addComponent(new Pontilus::ECS::Transform(
        //     glm::zero<glm::vec3>(),
        //     {100, 100, 0},
        //     {0.0, 0.0, 0.0}
        // ));
        // tmp_obj = obj->id;

        this->getCamera()->setMode(Pontilus::Renderer::Camera::PERSPECTIVE);
        this->getCamera()->setPosition(30.0, 30.0, 30.0);
    }

    void ChessScene::update(double dt) {
        constexpr float CAM_SPEED = 10.0f;
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_LEFT)) {
            this->getCamera()->move(-CAM_SPEED * dt, 0.0, 0.0);
        }
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_RIGHT)) {
            this->getCamera()->move(CAM_SPEED * dt, 0.0, 0.0);
        }
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_UP)) {
            this->getCamera()->move(0.0, CAM_SPEED * dt, 0.0);
        }
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_DOWN)) {
            this->getCamera()->move(0.0, -CAM_SPEED * dt, 0.0);
        }
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE)) {
            auto &pos = this->getCamera()->getPosition();
            printf("Camera Position: %f, %f, %f\n", pos.x, pos.y, pos.z);
        }
    }
    void ChessScene::clean() {}
} // BlockBlast