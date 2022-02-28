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

struct TileMap {
    Tile **_t;
    short *_block;
    int width, height, tilewidth;
    Tile &operator[](int i) {
        return _t[i];
    }
};

TileMap getTileMap(const short *_block, int width, int height, int tilewidth) {
    TileMap ret;
    ret.width = width;
    ret.height = height;
    ret.tilewidth = tilewidth;
    ret._block = (short *) malloc(sizeof(short) * width * height);
    ret._t = ()
// fix segfault here
    for (int i = 0; i < width * height; i++) {
        ret._t[i] = (Tile *) malloc(sizeof(Tile) * width * height);
        if (_block[i] >= 0)
            ret._block[i] = _block[i];
        else
            ret._block[i] = -1;
        
        int xOffset = (i % width * tilewidth);
        int yOffset = (floor(i / width) * tilewidth);
        if (ret._block[i]) {
            ret._t[i] = {};
            ret._t[i].init({xOffset, yOffset, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, tilewidth, tilewidth);
        }
    }
    return ret;
}

void deleteTileMap(TileMap &t) {
    free(t._block);
    free(t._t);
}

void applyIcons(TileMap &t, Graphics::IconMap &im) {
    for (int i = 0; i < t.width * t.height; i++) {
        if (t._block[i] >= 0) {
            Engine::ECS::SpriteRenderer r;
            Graphics::Texture tex = Graphics::getTexture(im, i);
            r.init(tex);
            t[i].addComponent(r);
        }
    }
}

class rect {
    public:
    glm::vec2 min, max;
};

rect rectFromObj(Engine::ECS::GameObject obj) {
    return {
        {obj.pos.x - obj.width / 2.0f, obj.pos.y - obj.height / 2.0f},
        {obj.pos.x + obj.width / 2.0f, obj.pos.y + obj.height / 2.0f},
    };
}

#define TILEMAP_WIDTH 10
#define TILEMAP_HEIGHT 10
#define NUM_TILES TILEMAP_WIDTH * TILEMAP_HEIGHT

static Player player;
static Engine::ECS::GameObject obj;
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::SpriteRenderer objRenderer;
static Engine::ECS::StateMachine playerController;
static Pontilus::Graphics::IconMap playerTextures;
static Pontilus::Graphics::IconMap tileTextures;

static const short tiles[TILEMAP_WIDTH][TILEMAP_HEIGHT] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

static TileMap tilemap = getTileMap(&tiles[0][0], 10, 10, 4);

typedef Pair<Tile, rect> tile_rect;

static bool detectRectRect(rect a, rect b)
{
    glm::vec2 awh = (a.max - a.min);
    rect r_t = {b.min - awh / 2.0f, b.max + awh / 2.0f};
    glm::vec2 a_c = (a.max + a.min) / 2.0f;
    
    return Math::between(r_t.min.x, a_c.x, r_t.max.x) &&
           Math::between(r_t.min.y, a_c.y, r_t.max.t);
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

        info.push_back({t, {min, max}});
    }
}

static bool collide() {
    // collision
    std::vector<tile_rect> info;
    getCollisionInfo(player, info);
    bool hasFloor = false;
    for (auto i : info) {
        float w = i.second.max.x - i.second.min.x;
        float h = i.second.max.y - i.second.min.y;
        if (w / player.width < h / player.height) {
            if (i.first.pos.x > player.pos.x) {
                if (player.velocity.x > 0.0f)
                    player.velocity.x = 0.0f;
                player.pos.x -= w;
            } else {
                if (player.velocity.x < 0.0f)
                    player.velocity.x = 0.0f;
                player.pos.x += w;
            }
        } else {
            player.velocity.y = 0.0f;
            if (i.first.pos.y > player.pos.y) {
                player.pos.y -= h;
            } else {
                player.pos.y += h;
                hasFloor = true;
            }
        }
    }
    return hasFloor;
}

static bool checkForFloor() {
    bool ret = false;
    for (int i = 0; i < NUM_TILES; i++) {
        rect foot = {
            {player.pos.x - player.width / 2.0f, player.pos.y - player.height / 2.0f - 0.1f},
            {player.pos.x + player.width / 2.0f, player.pos.y - player.height / 2.0f - 0.1f},
        };
        if (detectRectRect(foot, rectFromObj(tilemap[i]))) {
            ret = true;
            break;
        }
    }

    return ret;
}

static void horizontalMotion(double dt) {
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

        horizontalMotion(dt);
        
        collide();
        bool hasFloor = checkForFloor();
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
        
        horizontalMotion(dt);

        collide();
        bool hasFloor = checkForFloor();
        if (hasFloor) playerController.replaceState("jumped", "grounded");
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        // fall
        player.velocity -= glm::vec2{0.0f, 19.6f} * (float) dt;

        horizontalMotion(dt);
        
        collide();
        bool hasFloor = checkForFloor();
        if (hasFloor) playerController.replaceState("double-jumped", "grounded");
    }},
};

static Engine::Scene mainScene = {
    []() {
        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Graphics::initIconMap("./assets/textures/test2.png", tileTextures, 8, 8, 0);
        playerRenderer.init({nullptr});

        objRenderer.init({nullptr});
        

        for (int i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
            mainScene.objs.push_back(tilemap[i]);
        }
        
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 2.0f, 8.0f);
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
