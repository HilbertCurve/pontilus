#include <core/Application.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/StateMachine.h>

using namespace Pontilus;

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity = glm::vec2(0.0f);
};

class Sling : public Engine::ECS::GameObject {};

static Player player;
static Engine::ECS::SpriteRenderer r_player;
static Engine::ECS::StateMachine s_player;
static float GROUND = -8.0f;
static float GRAVITY = 80.0f;
static float SLING_CONST = 2.0f;

static Sling sling;
static Engine::ECS::SpriteRenderer r_sling;

static void horizMove(double dt, bool doFriction) {
    if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
        player.velocity.x -= 150.0f * dt;
        if (player.velocity.x < -32.0f) player.velocity.x = -32.0f;
    } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
        player.velocity.x += 150.0f * dt;
        if (player.velocity.x > 32.0f) player.velocity.x = 32.0f;
    } else {
        if (doFriction) {
            if (player.velocity.x < -1.0f) player.velocity.x += 150.0f * dt;
            else if (player.velocity.x > 1.0f) player.velocity.x -= 150.0f * dt;
            else player.velocity.x = 0.0f;
        }
    }
}

static bool isSpaceToggled = false;

static Engine::ECS::State states[] = {
    {"grounded", &s_player, [](double dt)
        {
            horizMove(dt, true);

            if (IO::isKeyPressed(GLFW_KEY_SPACE)) {
                player.velocity.y = 40.0f;
                s_player.replaceState("grounded", "airborne");
                isSpaceToggled = true;
                return;
            }
        }},
    {"airborne", &s_player, [](double dt)
        {
            player.velocity.y -= GRAVITY * dt;

            if (player.pos.y + player.velocity.y * dt <= GROUND) {
                player.velocity.y = 0.0f;
                player.pos.y = GROUND;

                s_player.replaceState("airborne", "grounded");
            } else if (IO::isKeyPressed(GLFW_KEY_SPACE) && !isSpaceToggled) {
                s_player.replaceState("airborne", "slinging");
            }

            if (!IO::isKeyPressed(GLFW_KEY_SPACE)) {
                isSpaceToggled = false;
            }
        }},
    {"slinging", &s_player, [](double dt)
        {
            glm::vec3 toSlingNormal = glm::normalize(sling.pos - player.pos);

            player.velocity += glm::vec2(toSlingNormal * SLING_CONST);

            if (player.pos.y + player.velocity.y * dt <= GROUND) {
                player.velocity.y = 0.0f;
                player.pos.y = GROUND;

                s_player.replaceState("slinging", "grounded");
            } else if (!IO::isKeyPressed(GLFW_KEY_SPACE)) {
                s_player.replaceState("slinging", "airborne");
                isSpaceToggled = false;
            }
        }}
};

Engine::Scene mainScene = {
    []() {
        // player
        player.init({0.0, 0.0, 0.0}, 4, 4);

        r_player.init({nullptr}, {1.0, 1.0, 1.0, 1.0});
        s_player.init(states, 3);

        player.addComponent(r_player);
        player.addComponent(s_player);

        mainScene.addObj(&player);
        
        // sling
        sling.init({0.0, 8.0, 0.0}, 2, 2);

        r_sling.init({nullptr}, {0.2, 0.4, 0.8, 0.65});

        sling.addComponent(r_sling);

        mainScene.addObj(&sling);

        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        player.pos += glm::vec3(player.velocity, 0.0) * (float) dt;

        // boilerplate, might remove at some point
        updateSceneGraphics(mainScene);
    },
    []() {

    }
};

int main() {
    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();

    return 0;
}

