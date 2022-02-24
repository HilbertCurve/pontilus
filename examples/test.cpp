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
#include <utils/PMath.h>

using namespace Pontilus;

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity;
};

class Tile : public Engine::ECS::GameObject {
    public:
    bool collides = true;
};

class rect {
    public:
    glm::vec2 min, max;
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

typedef Pair<Tile, rect> tile_rect;

static bool detectRectRect(rect a, rect b)
{
    return Math::overlap(a.min.x, a.max.x, b.min.x, b.max.x) > 0.0f &&
           Math::overlap(a.min.y, a.max.y, b.min.y, b.max.y) > 0.0f;
}

static void getCollisionInfo(Engine::ECS::GameObject &obj, std::vector<tile_rect> &info) {
    info.erase(info.begin(), info.end());

    glm::vec2 obj_v[4] = {
        glm::vec2(obj.pos) + glm::vec2{-obj.width - 0.1f,  obj.height + 0.1f} / 2.0f, 
        glm::vec2(obj.pos) + glm::vec2{ obj.width + 0.1f,  obj.height + 0.1f} / 2.0f,
        glm::vec2(obj.pos) + glm::vec2{ obj.width + 0.1f, -obj.height - 0.1f} / 2.0f, 
        glm::vec2(obj.pos) + glm::vec2{-obj.width - 0.1f, -obj.height - 0.1f} / 2.0f,
    };
    for (int i = 0; i < NUM_TILES; i++) {
        Tile t = tilemap[i]; // selection process can be optimized
        rect r_obj = {obj_v[3], obj_v[1]};
        rect r_t = {glm::vec2(t.pos) - glm::vec2{t.width, t.height} / 2.0f, glm::vec2(t.pos) + glm::vec2{t.width, t.height} / 2.0f};
        if (!detectRectRect(r_obj, r_t)) continue;
        
        glm::vec2 min = {fmax(r_obj.min.x, r_t.min.x), fmax(r_obj.min.y, r_t.min.y)};
        glm::vec2 max = {fmin(r_obj.max.x, r_t.max.x), fmin(r_obj.max.y, r_t.max.y)};

        info.push_back({t, {min, max}}); // TODO: collision resolution
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
        player.color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        // jump
        player.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;
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
            if (player.velocity.x < -1.0f) player.velocity.x += 100.0f * dt;
            else if (player.velocity.x > 1.0f) player.velocity.x -= 100.0f * dt;
            else player.velocity.x = 0.0f;
        }
        // collision
        bool hasFloor = false;
        float floorY = FLT_MAX;
        std::vector<tile_dir> info;
        getCollisionInfo(player, info);
        for (auto i : info) {
            if (i.second == SOUTH) {
                hasFloor = true;
                floorY = fmin(floorY, i.first.pos.y + i.first.height / 2);
            }
        }
        if (hasFloor && player.velocity.y <= 0.0f) {
            playerController.replaceState("jumped", "grounded");
            player.velocity.y = 0.0f;
            player.pos.y = floorY + player.height / 2;
        }
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        // fall
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
        float floorY = FLT_MAX;
        static const float FLOOR_THRESHOLD = 0.2f; 
        std::vector<tile_dir> info;
        
        getCollisionInfo(player, info);
        for (auto i : info) {
            if (i.second == SOUTH) {
                hasFloor = true;
                floorY = fmin(floorY, i.first.pos.y + i.first.height / 2);
            } else if (i.second == NORTH) {

            }
        }
        if (hasFloor && player.velocity.y <= 0.0f) {
            playerController.replaceState("double-jumped", "grounded");
            player.velocity.y = 0.0f;
            player.pos.y = floorY + player.height / 2;
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
            if (i > 10)
                tilemap[i].init({-50.0 + 4.0f * i, -10.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
            else
                tilemap[i].init({-30.0 + 4.0f * i, -20.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
            tilemap[i].addComponent(tileRenderer[i]);
            mainScene.objs.push_back(tilemap[i]);
        }
        
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 2.0f, 18.5f/2.5f);
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
