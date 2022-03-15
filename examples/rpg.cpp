#include <core/Application.h>
#include <core/InputListener.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/StateMachine.h>
#include <library/TileMap.h>

using namespace Pontilus;

static Engine::ECS::GameObject player;
static Engine::ECS::SpriteRenderer r_player;
static Engine::ECS::StateMachine c_player;
static Library::TileMap tilemap;
static Graphics::IconMap tilemap_icons;

enum direction {
    NORTH = 0b0001,
    EAST = 0b0010,
    SOUTH = 0b0100,
    WEST = 0b1000,
    NONE = 0b0000
};

#define TILEMAP_WIDTH 10
#define TILEMAP_HEIGHT 10
#define PLAYER_SPEED 10

int key[TILEMAP_HEIGHT][TILEMAP_WIDTH];

void move (direction dir, double dt) {
    static double t_acc;
    t_acc += dt;
    if (t_acc > 0.2) t_acc = 0.0;
}

static int dir;
static Engine::ECS::State p_states[] = {
    {"still", &c_player, [](double dt) {
        dir = NONE;
        if (IO::isKeyPressed(GLFW_KEY_W)) {
            dir |= NORTH;
            c_player.replaceState("still", "moving");
        }
        if (IO::isKeyPressed(GLFW_KEY_S)) {
            dir |= SOUTH;
            c_player.replaceState("still", "moving");
        }
        if (IO::isKeyPressed(GLFW_KEY_A)) {
            dir |= WEST;
            c_player.replaceState("still", "moving");
        }
        if (IO::isKeyPressed(GLFW_KEY_D)) {
            dir |= EAST;
            c_player.replaceState("still", "moving");
        }
        printf("still!\n");
    }},
    {"moving", &c_player, [](double dt) {
        static double t_acc = 0.0;
        t_acc += dt;
        if (t_acc > 0.2) {
            c_player.replaceState("moving", "still");
            t_acc = 0.0;
            
            // snap position
            player.pos = floor((player.pos + tilemap.tilewidth / 2.0f) / tilemap.tilewidth) * tilemap.tilewidth;

            return;
        }

        if (dir & NORTH)
            player.pos.y += tilemap.tilewidth * dt / 0.2; 
        if (dir & SOUTH)
            player.pos.y -= tilemap.tilewidth * dt / 0.2;
        if (dir & EAST)
            player.pos.x += tilemap.tilewidth * dt / 0.2;
        if (dir & WEST)
            player.pos.x -= tilemap.tilewidth * dt / 0.2; 
        
        printf("dir: %d\n", dir);
    }}
};

Engine::Scene mainScene = {
    []() {
        player = Engine::ECS::GameObject();
        player.init({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);

        // tilemap
        Graphics::initIconMap("./assets/textures/tilemap1.png", tilemap_icons, 16, 16, 0);

        for (int i = 0; i < TILEMAP_HEIGHT * TILEMAP_WIDTH; i++) {
            (&(key[0][0]))[i] = -1;
        }

        Library::getTileMap(TILEMAP_WIDTH, TILEMAP_HEIGHT, &key[0][0], tilemap, 4, &tilemap_icons);

        r_player.init({nullptr});
        c_player.init(&p_states[0], 2);

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