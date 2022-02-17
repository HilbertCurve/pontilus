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

class Tile : public Engine::ECS::GameObject {
    public:
    bool collides = true;
};

#define NUM_TILES 2

static Player player;
static Tile tilemap[NUM_TILES] = {Tile(), Tile()};
static Engine::ECS::SpriteRenderer tileRenderer[NUM_TILES];
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::StateMachine playerController;
static Pontilus::Graphics::IconMap playerTextures;
static Pontilus::Graphics::IconMap tileTextures;

static int collidingWithTile() {
    for (int i = 0; i < NUM_TILES; i++) {
        if (player.pos.y - player.height / 2.0f <= tilemap[i].pos.y + tilemap[i].height / 2.0f &&
            player.pos.y + player.height / 2.0f >= tilemap[i].pos.y - tilemap[i].height / 2.0f &&
            player.pos.x - player.width  / 2.0f <= tilemap[i].pos.x + tilemap[i].width  / 2.0f &&
            player.pos.x + player.width  / 2.0f >= tilemap[i].pos.x - tilemap[i].width  / 2.0f) return i;
    }
    return -1;
}

static bool curr = false, prev = false;
static Engine::ECS::State sControllers[] = {
    {"grounded", &playerController, [](double dt) {
        // jump
        player.color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("jumped");
            playerController.removeState("grounded");
            player.velocity.y = 20.0f;
        }
        prev = curr;
        // move
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            player.velocity.x -= 100.0f * dt;
            if (player.velocity.x < -16.0f) player.velocity.x = -16.0f;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            player.velocity.x += 100.0f * dt;
            if (player.velocity.x > 16.0f) player.velocity.x = 16.0f;
        } else {
            if (player.velocity.x < -1.0f) player.velocity.x += 100.0f * dt;
            else if (player.velocity.x > 1.0f) player.velocity.x -= 100.0f * dt;
            else player.velocity.x = 0.0f;
        }
        /*
        if (int tile = collidingWithTile() == -1) {
            if (player.velocity.y < 0.0f) {
                player.pos.y = tilemap[tile].pos.y + tilemap[tile].height / 2;
                playerController.setState("grounded");
            }
        }
        */
    }},
    {"jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("double-jumped");
            playerController.removeState("jumped");
            player.velocity.y = 15.0f;
        } else {
            player.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
            if (player.pos.y < 0.0f) {
                player.pos.y = 0.0f;
                player.velocity.y = 0.0f;
                playerController.addState("grounded");
                playerController.removeState("jumped");
            }
        }
        prev = curr;
        // move
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            player.velocity.x -= 100.0f * dt;
            if (player.velocity.x < -16.0f) player.velocity.x = -16.0f;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            player.velocity.x += 100.0f * dt;
            if (player.velocity.x > 16.0f) player.velocity.x = 16.0f;
        } else {
            if (player.velocity.x < -1.0f) player.velocity.x += 100.0f * dt;
            else if (player.velocity.x > 1.0f) player.velocity.x -= 100.0f * dt;
            else player.velocity.x = 0.0f;
        }
        //if (collidingWithTile() && player.velocity.x != 0.0f) player.velocity.x = 0.0f;
        //if (collidingWithTile() && player.velocity.y != 0.0f) player.velocity.y = 0.0f;
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        player.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
        if (player.pos.y < 0.0f) {
            player.pos.y = 0.0f;
            player.velocity.y = 0.0f;
            playerController.addState("grounded");
            playerController.removeState("double-jumped");
        }
        // move
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            player.velocity.x -= 100.0f * dt;
            if (player.velocity.x < -16.0f) player.velocity.x = -16.0f;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            player.velocity.x += 100.0f * dt;
            if (player.velocity.x > 16.0f) player.velocity.x = 16.0f;
        } else {
            if (player.velocity.x < -1.0f) player.velocity.x += 100.0f * dt;
            else if (player.velocity.x > 1.0f) player.velocity.x -= 100.0f * dt;
            else player.velocity.x = 0.0f;
        }
        //if (collidingWithTile() && player.velocity.x != 0.0f) player.velocity.x = 0.0f;
        //if (collidingWithTile() && player.velocity.y != 0.0f) player.velocity.y = 0.0f;
    }},
};

static Engine::Scene mainScene = {
    []() {

        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Graphics::initIconMap("./assets/textures/test2.png", tileTextures, 8, 8, 0);
        playerRenderer.init(Pontilus::Graphics::getTexture(playerTextures, 0));
        for (int i = 0; i < 2; i++) {
            tileRenderer[i] = Engine::ECS::SpriteRenderer();
            tileRenderer[i].init(Pontilus::Graphics::getTexture(tileTextures, i));
            tilemap[i] = Tile();
            tilemap[i].init({10.0 + 4.0f * i, 10.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
            tilemap[i].addComponent(tileRenderer[i]);
            mainScene.objs.push_back(tilemap[i]);
        }
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 8.0f, 18.5f/2.5f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);

        mainScene.objs.push_back(player);
        
        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        glm::vec2 motion = player.velocity * (float) dt;
        player.pos += glm::vec3{motion.x, motion.y, 0.0f};
        updateSceneGraphics(mainScene);
    },
    []() {

    }
};

int main() 
{
    Pontilus::setEcho(true);
    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();
}
