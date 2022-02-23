#include <cstdio>
#include <core/Application.h>
#include <core/Scene.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/Component.h>
#include <ecs/StateMachine.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/Body2D.h>
#include <physics2d/CollisionDetection.h>
#include <math.h>

using namespace Pontilus;

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity;
};

class Tile : public Engine::ECS::GameObject {
    public:
    bool collides = true;
};

#define NUM_TILES 20

static Player player;
static Engine::ECS::GameObject obj;
static Tile tilemap[NUM_TILES] = {Tile(), Tile()};
static Engine::ECS::SpriteRenderer tileRenderer[NUM_TILES];
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::SpriteRenderer objRenderer;
static Engine::ECS::StateMachine playerController;
static Pontilus::Graphics::IconMap playerTextures;
static Pontilus::Graphics::IconMap tileTextures;

enum direction {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

typedef Pair<Tile, direction> tile_dir;

static bool detectPointTile(glm::vec2 p, Tile t) {
    bool insideX = p.x >= t.pos.x - t.width/2 && p.x <= t.pos.x + t.width/2;
    bool insideY = p.y >= t.pos.y - t.height/2 && p.y <= t.pos.y + t.height/2;

    return insideX && insideY;
}

static void getCollisionInfo(Engine::ECS::GameObject &obj, std::vector<tile_dir> &info) {
    info.erase(info.begin(), info.end());

    for (int i = 0; i < NUM_TILES; i++) {
        Tile t = tilemap[i]; // selection process can be optimized
        glm::vec2 pts[4] = { 
            obj.pos + glm::vec3{0.0f, obj.height / 2 + 0.1f, 0.0f},
            obj.pos + glm::vec3{obj.width / 2 + 0.1f,  0.0f, 0.0f},
            obj.pos - glm::vec3{0.0f, obj.height / 2 + 0.1f, 0.0f},
            obj.pos - glm::vec3{obj.width / 2 + 0.1f,  0.0f, 0.0f}};

        for (int i = 0; i < 4; i++) {
            if (detectPointTile(pts[i], t)) {
                info.push_back(tile_dir{t, (direction) i});
            }
        }
    }
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
        // collision
        std::vector<tile_dir> info;
        getCollisionInfo(player, info);
        bool hasFloor = false;
        for (auto i : info) {
            if (i.second == SOUTH) hasFloor = true;
        }
        if (!hasFloor) playerController.replaceState("grounded", "jumped");
    }},
    {"jumped", &playerController, [](double dt) {
        // jump
        player.color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("double-jumped");
            playerController.removeState("jumped");
            player.velocity.y = 15.0f;
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
            //if (player.velocity.x < -1.0f) player.velocity.x += 100.0f * dt;
            //else if (player.velocity.x > 1.0f) player.velocity.x -= 100.0f * dt;
            //else player.velocity.x = 0.0f;
        }
        // collision
        bool hasFloor = false;
        float floorY = FLT_MAX;
        std::vector<tile_dir> info;
        getCollisionInfo(player, info);
        for (auto i : info) {
            if (i.second == SOUTH) {
                hasFloor = true;
                floorY = fmin(floorY, i.first.pos.y - i.first.height / 2);
            }
        }
        if (hasFloor) {
            playerController.replaceState("jumped", "grounded");
            player.velocity.y = 0.0f;
            player.pos.y = floorY + player.height / 2;
        }
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        player.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
        
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
        // collision
        bool hasFloor = false;
        std::vector<tile_dir> info;
        getCollisionInfo(player, info);
        for (auto i : info) {
            if (i.second == SOUTH) hasFloor = true;
        }
        if (hasFloor) {
            playerController.replaceState("double-jumped", "grounded");
            player.velocity.y = 0.0f;
        }
    }},
};

static Engine::Scene mainScene = {
    []() {
        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Graphics::initIconMap("./assets/textures/test2.png", tileTextures, 8, 8, 0);
        playerRenderer.init({nullptr});

        objRenderer.init({nullptr});
        
        for (int i = 0; i < NUM_TILES; i++) {
            tileRenderer[i] = Engine::ECS::SpriteRenderer();
            tileRenderer[i].init(Pontilus::Graphics::getTexture(tileTextures, i));
            tilemap[i] = Tile();
            tilemap[i].init({-10.0 + 4.0f * i, -10.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
            tilemap[i].addComponent(tileRenderer[i]);
            mainScene.objs.push_back(tilemap[i]);
        }
        
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 8.0f, 18.5f/2.5f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);

        obj.init({0.0f, 20.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, 4.0f, 4.0f);
        obj.addComponent(objRenderer);

        mainScene.objs.push_back(player);
        mainScene.objs.push_back(obj);
        
        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        player.pos += glm::vec3(player.velocity * (float) dt, 0.0f);
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
