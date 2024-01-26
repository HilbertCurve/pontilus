#include "editor.h"
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
    static int playerID, tilemapID, editorID;

    static Scene primary = {
        []() {
            ECS::GameObject &player = primary.spawn();

            player.addComponent(new ECS::Transform({0.0, 2.0, -1.0}, {2.0, 4.0, 1.0}, {0.0, 0.0, 0.0}));
            player.addComponent(&Platformer::Player::get());
            player.addComponent(new Renderer::SpriteRenderer({0.0, 0.0, 1.0, 1.0}));

            ECS::GameObject &tilemap = primary.spawn();

            tilemap.addComponent(new ECS::Transform({0.0, 0.0, -1.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}));
            tilemap.addComponent(new Platformer::TileMap(2.0f, 2.0f, -1.0f));

            TileMap &t = *(TileMap *)tilemap.getComponent(typeid(TileMap));
            t.setTile({0,0},0);
            t.setTile({0,-1},0);
            t.setTextures("../assets/textures/tilemap1.png", 16, 16, 0);

            ECS::GameObject &editor = primary.spawn();

            editor.addComponent(new ECS::Transform({0.0, 0.0, -1.0}, {2.0, 2.0, 1.0}, {0.0, 0.0, 0.0}));
            editor.addComponent(&Platformer::Editor::get());
            editor.addComponent(new Renderer::SpriteRenderer({0.5, 0.5, 0.5, 1.0}));

            Platformer::Editor::get().edit(t);
            Platformer::Player::get().collideWith(t);

            playerID = player.id;
            tilemapID = tilemap.id;
            editorID = editor.id;
        },
        [](double) {

        },
        []() {

        }
    };
}

int main() {
    Pontilus::Application *app = Pontilus::Application::get();
    app->getMainWindow().setScene(Platformer::primary);
    app->run();
    return 0;
}
