#include "player.h"

#include <core/Application.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/Transform.h>

using namespace Pontilus::Engine::ECS;
using namespace Pontilus::Engine;



namespace Platformer
{
    static Scene primary = {
        []() {
            GameObject &player = primary.spawn();

            player.addComponent(new Transform({0.0, 0.0, -1.0}, {4.0, 4.0, 1.0}, {0.0, 0.0, 0.0}));
            player.addComponent(&Platformer::Player::get());
            player.addComponent(new SpriteRenderer({0.0, 0.0, 1.0, 1.0}));
        },
        [](double) {

        },
        []() {

        }
    };
}

int main() {
    Pontilus::init();
    Pontilus::setCurrentScene(Platformer::primary);
    Pontilus::loop();
    return 0;
}