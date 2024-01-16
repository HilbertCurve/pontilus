#include "player.h"
#include "tilemap.h"

#include <core/Application.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <renderer/SpriteRenderer.h>
#include <ecs/Transform.h>

using namespace Pontilus;

// next: TileMaps with textures
// then: builder mode + saving game
// then: enemies
// then: interactables
// then: levels
// then: title
// then: have fun!

namespace Platformer
{
    static Scene primary = {
        []() {
            ECS::GameObject &player = primary.spawn();

            player.addComponent(new ECS::Transform({0.0, 0.0, -1.0}, {2.0, 5.0, 1.0}, {0.0, 0.0, 0.0}));
            player.addComponent(&Platformer::Player::get());
            player.addComponent(new Renderer::SpriteRenderer({0.0, 0.0, 1.0, 1.0}));

            ECS::GameObject &tilemap = primary.spawn();

            tilemap.addComponent(new ECS::Transform({0.0, 0.0, -1.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}));
            tilemap.addComponent(new Platformer::TileMap(2.0f, 2.0f, -1.0f));

            TileMap &t = *(TileMap *)tilemap.getComponent(typeid(TileMap));
            for (int i = 0; i < 10; i++)
                t.setTile({i, 0}, 1);
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