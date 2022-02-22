#include <cstdio>
#include <core/Application.h>
#include <core/Scene.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/Component.h>
#include <ecs/StateMachine.h>
#include <ecs/SpriteRenderer.h>
#include <physics2d/CollisionDetection.h>
#include <physics2d/Body2D.h>

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
static Engine::ECS::GameObject obj;
static Physics2D::AABB playerBody;
static Physics2D::AABB objBody;
static Tile tilemap[NUM_TILES] = {Tile(), Tile()};
static Engine::ECS::SpriteRenderer tileRenderer[NUM_TILES];
static Physics2D::AABB tileCollider[NUM_TILES];
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::SpriteRenderer objRenderer;
static Engine::ECS::StateMachine playerController;
static Pontilus::Graphics::IconMap playerTextures;
static Pontilus::Graphics::IconMap tileTextures;

static bool curr = false, prev = false;
static Engine::ECS::State sControllers[] = {
    {"grounded", &playerController, [](double dt) {
        // jump
        player.color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("jumped");
            playerController.removeState("grounded");
            playerBody.velocity.y = 20.0f;
        }
        prev = curr;
        // move
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            playerBody.velocity.x -= 100.0f * dt;
            if (playerBody.velocity.x < -16.0f) playerBody.velocity.x = -16.0f;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            playerBody.velocity.x += 100.0f * dt;
            if (playerBody.velocity.x > 16.0f) playerBody.velocity.x = 16.0f;
        } else {
            if (playerBody.velocity.x < -1.0f) playerBody.velocity.x += 100.0f * dt;
            else if (playerBody.velocity.x > 1.0f) playerBody.velocity.x -= 100.0f * dt;
            else playerBody.velocity.x = 0.0f;
        }
        /*
        if (int tile = collidingWithTile() == -1) {
            if (playerBody.velocity.y < 0.0f) {
                playerBody.pos.y = tilemap[tile].pos.y + tilemap[tile].height / 2;
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
            playerBody.velocity.y = 15.0f;
        } else {
            //playerBody.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
            if (playerBody.center.y < 0.0f && playerBody.velocity.y < 0.0f) {
                playerBody.velocity.y = 0.0f;
                playerController.addState("grounded");
                playerController.removeState("jumped");
            }
        }
        prev = curr;
        // move
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            playerBody.velocity.x -= 100.0f * dt;
            if (playerBody.velocity.x < -16.0f) playerBody.velocity.x = -16.0f;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            playerBody.velocity.x += 100.0f * dt;
            if (playerBody.velocity.x > 16.0f) playerBody.velocity.x = 16.0f;
        } else {
            if (playerBody.velocity.x < -1.0f) playerBody.velocity.x += 100.0f * dt;
            else if (playerBody.velocity.x > 1.0f) playerBody.velocity.x -= 100.0f * dt;
            else playerBody.velocity.x = 0.0f;
        }
        //if (collidingWithTile() && playerBody.velocity.x != 0.0f) playerBody.velocity.x = 0.0f;
        //if (collidingWithTile() && playerBody.velocity.y != 0.0f) playerBody.velocity.y = 0.0f;
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        playerBody.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
        if (playerBody.center.y < 0.0f) {
            playerBody.velocity.y = 0.0f;
            playerController.addState("grounded");
            playerController.removeState("double-jumped");
        }
        // move
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            playerBody.velocity.x -= 100.0f * dt;
            if (playerBody.velocity.x < -16.0f) playerBody.velocity.x = -16.0f;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            playerBody.velocity.x += 100.0f * dt;
            if (playerBody.velocity.x > 16.0f) playerBody.velocity.x = 16.0f;
        } else {
            if (playerBody.velocity.x < -1.0f) playerBody.velocity.x += 100.0f * dt;
            else if (playerBody.velocity.x > 1.0f) playerBody.velocity.x -= 100.0f * dt;
            else playerBody.velocity.x = 0.0f;
        }
        //if (collidingWithTile() && playerBody.velocity.x != 0.0f) playerBody.velocity.x = 0.0f;
        //if (collidingWithTile() && playerBody.velocity.y != 0.0f) playerBody.velocity.y = 0.0f;
    }},
};

static Engine::Scene mainScene = {
    []() {
        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Graphics::initIconMap("./assets/textures/test2.png", tileTextures, 8, 8, 0);
        playerRenderer.init({nullptr});
        playerBody = Physics2D::AABB({0.0f - 4.0f, 1.0f - 18.5f/5.0f}, {0.0f + 4.0f, 1.0f + 18.5f/5.0f});
        playerBody.init();
        playerBody.mass = IMMOVABLE;

        objRenderer.init({nullptr});
        objBody = Physics2D::AABB({-2.0f, 18.0f}, {2.0f, 22.0f});
        objBody.init();
        objBody.mass = 50.0f;
        /*
        for (int i = 0; i < 2; i++) {
            tileRenderer[i] = Engine::ECS::SpriteRenderer();
            tileRenderer[i].init(Pontilus::Graphics::getTexture(tileTextures, i));
            tileCollider[i] = Physics2D::AABB(glm::vec2{0.0 + 8.0f * i - 1.0f, 10.0f - 1.0f}, glm::vec2{10.0 + 4.0f * i, 10.0f} + 1.0f);
            tileCollider[i].init();
            tileCollider[i].mass = 4.0f;
            tilemap[i] = Tile();
            tilemap[i].init({10.0 + 4.0f * i, 10.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
            tilemap[i].addComponent(tileRenderer[i]);
            tilemap[i].addComponent(tileCollider[i]);
            //ainScene.objs.push_back(tilemap[i]);
        }
        */
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 8.0f, 18.5f/2.5f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);
        player.addComponent(playerBody);

        obj.init({0.0f, 20.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, 4.0f, 4.0f);
        obj.addComponent(objRenderer);
        obj.addComponent(objBody);

        mainScene.objs.push_back(player);
        mainScene.objs.push_back(obj);
        
        updateSceneGraphics(mainScene);
    },
    [](double dt) {

        objBody.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
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
