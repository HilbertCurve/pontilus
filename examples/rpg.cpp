#include <core/Application.h>
#include <core/InputListener.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/StateMachine.h>

using namespace Pontilus;

static Engine::ECS::GameObject player;
static Engine::ECS::SpriteRenderer r_player;
static Engine::ECS::StateMachine c_player;

#define PLAYER_SPEED 10

static Engine::ECS::State p_states[] = {
    {"standard", &c_player, [](double dt) {
        if (IO::isKeyPressed(GLFW_KEY_W)) {
            player.pos.y += PLAYER_SPEED * dt;
        }
        if (IO::isKeyPressed(GLFW_KEY_S)) {
            player.pos.y -= PLAYER_SPEED * dt;
        }
        if (IO::isKeyPressed(GLFW_KEY_A)) {
            player.pos.x -= PLAYER_SPEED * dt;
        }
        if (IO::isKeyPressed(GLFW_KEY_D)) {
            player.pos.x += PLAYER_SPEED * dt;
        }
    }}
};

Engine::Scene mainScene = {
    []() {
        player = Engine::ECS::GameObject();
        player.init({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);

        r_player.init({nullptr});
        c_player.init(&p_states[0], 1);

        player.addComponent(r_player);
        player.addComponent(c_player);

        mainScene.addObj(&player);

        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        updateSceneGraphics(mainScene);
    },
    []() {

    }
};

int main() {
    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();
}