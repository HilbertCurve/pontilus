#include <cstdio>
#include <core/Application.h>
#include <core/Scene.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/Component.h>
#include <ecs/StateMachine.h>
#include <ecs/SpriteRenderer.h>

using namespace Pontilus;

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity;
};

static Player player;
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::StateMachine playerController;
static Pontilus::Graphics::IconMap playerTextures;

static Engine::ECS::State sControllers[] = {
    {"grounded", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE)) {
            playerController.setState("jumped");
            player.velocity = glm::vec2{0.0f, 6.0f};
            printf("hello\n");
        }
    }},
    {"jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE) && Pontilus::IO::isKeyPressed(GLFW_KEY_R)) {
            playerController.setState("double-jumped");
            player.velocity = glm::vec2{0.0f, 6.0f};
            printf("hello\n");
        } else {
            player.velocity -= glm::vec2{0.0f, 9.8f} * (float) dt;
            if (player.pos.y < 0.0f) {
                player.pos.y = 0.0f;
                player.velocity.y = 0.0f;
                playerController.setState("grounded");
            }
        }
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        player.velocity -= glm::vec2{0.0f, 9.8f} * (float) dt;
        if (player.pos.y < 0.0f) {
            player.pos.y = 0.0f;
            player.velocity.y = 0.0f;
            playerController.setState("grounded");
        }
    }},
};

static Engine::Scene mainScene = {
    []() {
        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        playerRenderer.init(Pontilus::Graphics::getTexture(playerTextures, 0));
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 20.0f, 18.5f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);

        mainScene.objs.push_back(player);
        
        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        player.pos.y += player.velocity.y * (float) dt;
        updateSceneGraphics(mainScene);
    },
    []() {

    }
};

int main()
{
    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();
}