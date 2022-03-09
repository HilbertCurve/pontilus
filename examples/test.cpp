#include <cstdio>
#include <audio/AudioMaster.h>
#include <core/Application.h>
#include <core/Scene.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/Component.h>
#include <ecs/StateMachine.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/Body2D.h>
#include <ecs/AudioListener.h>
#include <ecs/AudioSource.h>
#include <graphics/Camera.h>
#include <physics2d/CollisionDetection.h>
#include <utils/PMath.h>
#include <utils/Utils.h>

using namespace Pontilus;

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity;
};

class Tile : public Engine::ECS::GameObject {
    public:
    bool collides = true;
    glm::vec<2, int> tile_coords;
};

class rect {
    public:
    glm::vec2 min, max;
};

class TileMap {
    public:
    std::vector<Tile> tiles;
    std::vector<Engine::ECS::SpriteRenderer> renderers;
    int *key;
    float tilewidth;
    int width, height;
    glm::vec4 color;
    Graphics::IconMap *tileset;
    size_t size() {
        return tiles.size();
    }
    Tile &operator[](int i) {
        return tiles[i];
    }
    Tile &at(int i) {
        return tiles.at(i);
    }

};

rect rectFromObj(Engine::ECS::GameObject obj) {
    return {
        {obj.pos.x - obj.width / 2.0f, obj.pos.y - obj.height / 2.0f},
        {obj.pos.x + obj.width / 2.0f, obj.pos.y + obj.height / 2.0f},
    };
}

#define TILEMAP_WIDTH 10
#define TILEMAP_HEIGHT 20
#define NUM_TILES tilemap.size()

static Player player;
static Engine::ECS::GameObject obj;
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::SpriteRenderer objRenderer;
static Engine::ECS::StateMachine playerController;
static Engine::ECS::AudioSource playerSource;
static Pontilus::Graphics::IconMap playerTextures;
static Pontilus::Graphics::IconMap tileTextures;
static Pontilus::Audio::WAVFile jump1, jump2;

static int key[TILEMAP_HEIGHT][TILEMAP_WIDTH] = {
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, 15, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, 15, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, 15, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { 15, 15, 15, 15, 15, 15, 15, 15, -1, -1,  },
    { 15, 15, 15, -1, -1, -1, -1, -1, -1, -1,  },
    { 15, 15, 15, -1, 15, -1, -1, -1, -1, -1,  },
    { 15, 15, 15, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, 15, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, 15, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, 15, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, 15, -1, -1, -1, -1, -1, -1, -1,  },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  },
    { 12,  9,  9,  9,  9,  9,  9,  9,  9, 11,  },
};



TileMap tilemap = TileMap();

typedef Pair<Tile, rect> tile_rect;

void getTileMap(unsigned n, unsigned k, TileMap &t, float tilewidth, Graphics::IconMap *tileset) {
    // empty tiles
    t.tiles.clear();
    t.renderers.clear();
    t.tilewidth = tilewidth;
    t.width = n;
    t.height = k;
    t.color = {1.0f, 1.0f, 1.0f, 1.0f};
    t.tileset = tileset;
    using namespace Engine::ECS;
    // loop through key, inserting tiles if key[i + j*n] >= 0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            int tiletex = t.key[i + j * n];
            if (tiletex >= 0) {
                Tile tile = Tile();
                tile.init({i * tilewidth, (k - (j + 1)) * tilewidth, 0.0f}, t.color, tilewidth, tilewidth);

                tile.tile_coords = {i, (k - (j + 1))};

                SpriteRenderer s = SpriteRenderer();
                if (tileset) {
                    s.init(Graphics::getTexture(*tileset, tiletex));
                } else {
                    s.init({nullptr});
                }

                t.tiles.push_back(tile);
                t.renderers.push_back(s);
            }
        }
    }
}

glm::vec2 tileCoords(float x, float y, float tilewidth) {
    return {floor((x + tilewidth / 2) / tilewidth), floor((y + tilewidth / 2) / tilewidth)};
}

void addTile(TileMap &t, glm::vec<2, int> coords, float tile) {
    int i_tile = floor(tile);
    if (!Math::between({-1, -1}, coords, {t.width + 1, t.height + 1})) {
        return;
    }

    for (int i = 0; i < t.size(); i++) {
        if (t[i].tile_coords == coords) {
            return;
        }
    }

    Tile _tile;
    _tile.init({coords.x * t.tilewidth, (coords.y) * t.tilewidth, 0.0f}, t.color, t.tilewidth, t.tilewidth);

    _tile.tile_coords = coords;
    Engine::ECS::SpriteRenderer _renderer = Engine::ECS::SpriteRenderer();
    _renderer.init(Graphics::getTexture(*t.tileset, i_tile));

    t.tiles.push_back(_tile);
    t.renderers.push_back(_renderer);

    t.at(t.size() - 1).addComponent(t.renderers.at(t.size() - 1));
    Pontilus::getCurrentScene()->objs.push_back(&t.at(t.size() - 1));
    // TODO: insert value tile into t.key
}

void applyColorFilter(TileMap &t, glm::vec4 color) {
    for (Tile &tile : t.tiles) {
        tile.color = color;
    }
    t.color = color;
}

static bool detectRectRect(rect a, rect b)
{
    glm::vec2 awh = (a.max - a.min);
    rect r_t = {b.min - awh / 2.0f, b.max + awh / 2.0f};
    glm::vec2 a_c = (a.max + a.min) / 2.0f;
    
    return Math::between(r_t.min.x, a_c.x, r_t.max.x) &&
           Math::between(r_t.min.y, a_c.y, r_t.max.t);
}

static void getCollisionInfo(Engine::ECS::GameObject &obj, std::vector<tile_rect> &info) {
    info.clear();

    glm::vec2 obj_v[4] = {
        glm::vec2(obj.pos) + glm::vec2{-obj.width,  obj.height} / 2.0f, 
        glm::vec2(obj.pos) + glm::vec2{ obj.width,  obj.height} / 2.0f,
        glm::vec2(obj.pos) + glm::vec2{ obj.width, -obj.height} / 2.0f, 
        glm::vec2(obj.pos) + glm::vec2{-obj.width, -obj.height} / 2.0f,
    };

    std::vector<Tile> surrounding;
    surrounding.clear();
    glm::vec2 p_tileCoordsMin, p_tileCoordsMax;
    p_tileCoordsMin = floor((obj_v[3] - tilemap.tilewidth / 2) / tilemap.tilewidth) * tilemap.tilewidth;
    p_tileCoordsMax = ceil((obj_v[1] + tilemap.tilewidth / 2) / tilemap.tilewidth) * tilemap.tilewidth;
    
    for (int i = 0; i < tilemap.size(); i++) {
        if (Math::between(p_tileCoordsMin, tilemap[i].pos, p_tileCoordsMax)) surrounding.push_back(tilemap[i]);
    }

    for (int i = 0; i < surrounding.size(); i++) {
        Tile t = surrounding[i];
        rect r_obj = {obj_v[3], obj_v[1]};
        rect r_t = {glm::vec2(t.pos) - glm::vec2{t.width, t.height} / 2.0f, glm::vec2(t.pos) + glm::vec2{t.width, t.height} / 2.0f};
        if (!detectRectRect(r_obj, r_t)) continue;
        
        glm::vec2 min = {fmax(r_obj.min.x, r_t.min.x), fmax(r_obj.min.y, r_t.min.y)};
        glm::vec2 max = {fmin(r_obj.max.x, r_t.max.x), fmin(r_obj.max.y, r_t.max.y)};

        info.push_back({t, {min, max}});
    }
}

static void collide(double dt) {
    // collision
    std::vector<tile_rect> info;
    getCollisionInfo(player, info);
    bool hasFloor = false;
    bool hasLeft, hasRight, hasUp;
    for (auto i : info) {
        float w = i.second.max.x - i.second.min.x;
        float h = i.second.max.y - i.second.min.y;
        
        // if it's a small corner overlap between tile on a wall and player, player 
        // could get caught on the top of one tile of the wall.
        if (w * h < __pEPSILON * 50 /* adjust */) continue;

        if (w / player.width < h / player.height) {
            if (i.first.pos.x > player.pos.x) {
                if (player.velocity.x > 0.0f)
                    player.velocity.x = 0.0f;
                if (!hasRight) {
                    player.pos.x -= w;
                    hasRight = true;
                }
            } else {
                if (player.velocity.x < 0.0f)
                    player.velocity.x = 0.0f;
                if (!hasLeft) { 
                    player.pos.x += w;
                    hasLeft = true;
                }
            }
        } else {
            if (i.first.pos.y > player.pos.y) {
                player.velocity.y = -0.1f;
                if (!hasUp) {
                    player.pos.y -= h;
                    hasUp = true;
                }
            } else if (!hasFloor) {
                player.velocity.y = 0.0f;
                player.pos.y += h;
                hasFloor = true;
            }
        }
    }
}

static bool checkForFloor(std::vector<tile_rect> &t) {
    bool ret = false;
    t.clear();
    for (int i = 0; i < tilemap.size(); i++) {
        rect foot = {
            {player.pos.x - player.width / 2.0f, player.pos.y - player.height / 2.0f - 0.1f},
            {player.pos.x + player.width / 2.0f, player.pos.y - player.height / 2.0f - 0.1f},
        };
        if (detectRectRect(foot, rectFromObj(tilemap[i]))) {
            ret = true;
            t.push_back({tilemap[i], {}});
            break;
        }
    }

    return ret;
}

static void horizontalMotion(double dt) {
    // move
    if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
        player.velocity.x -= 100.0f * dt;
        if (player.velocity.x < -32.0f) player.velocity.x = -32.0f;
    } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
        player.velocity.x += 100.0f * dt;
        if (player.velocity.x > 32.0f) player.velocity.x = 32.0f;
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
            player.velocity.y = 40.0f;
            Engine::ECS::AudioSource *ptr = (Engine::ECS::AudioSource *) player.getComponent(typeid(Engine::ECS::AudioSource));
            ptr->play(jump1, false);
        }
        prev = curr;

        horizontalMotion(dt);
        player.pos += glm::vec3(player.velocity * (float) dt, 0.0f);
        
        collide(dt);
        std::vector<tile_rect> info;
        bool hasFloor = checkForFloor(info);
        if (!hasFloor) {
            playerController.replaceState("grounded", "jumped");
        }
    }},
    {"jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        // jump
        player.velocity -= glm::vec2{0.0f, 39.6f} * (float) dt;
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("double-jumped");
            playerController.removeState("jumped");
            player.velocity.y = 30.0f;
            Engine::ECS::AudioSource *ptr = (Engine::ECS::AudioSource *) player.getComponent(typeid(Engine::ECS::AudioSource));
            ptr->play(jump1, false);
        }
        prev = curr;
        
        horizontalMotion(dt);
        player.pos += glm::vec3(player.velocity * (float) dt, 0.0f);

        collide(dt);
        std::vector<tile_rect> info;
        bool hasFloor = checkForFloor(info);
        if (hasFloor && player.velocity.y <= 0.0f) {
            playerController.replaceState("jumped", "grounded");
            //Tile floor = info.at(0).first;
            //player.pos.y = floor.pos.y + (floor.height - player.height) / 2.0f;
        }
    }},
    {"double-jumped", &playerController, [](double dt) {
        player.color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        // fall
        player.velocity -= glm::vec2{0.0f, 39.6f} * (float) dt;

        horizontalMotion(dt);
        player.pos += glm::vec3(player.velocity * (float) dt, 0.0f);
        
        collide(dt);
        std::vector<tile_rect> info;
        bool hasFloor = checkForFloor(info);
        if (hasFloor && player.velocity.y <= 0.0f) {
            playerController.replaceState("double-jumped", "grounded");
            //Tile floor = info.at(0).first;
            //player.pos.y = floor.pos.y + (floor.height - player.height) / 2.0f;
        }
    }},
};

float selectedBlock = 0;
const float maxSelectedBlock = 30;

static Engine::Scene mainScene = {
    []() {
        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Graphics::initIconMap("./assets/textures/tilemap1.png", tileTextures, 16, 16, 0);
        Pontilus::Audio::initWAVFile(jump1, "./assets/sounds/jump1.wav");

        playerRenderer.init({nullptr}/*Graphics::getTexture(playerTextures, 0)*/);
        playerSource.init();

        objRenderer.init({nullptr});
        tilemap.key = &key[0][0];
        getTileMap(TILEMAP_WIDTH, TILEMAP_HEIGHT, tilemap, 8, &tileTextures);
        applyColorFilter(tilemap, {0.5f, 0.5f, 0.5f, 0.5f});

        for (int i = 0; i < tilemap.size(); i++) {
            tilemap.at(i).addComponent(tilemap.renderers.at(i));
            mainScene.objs.push_back(&tilemap.at(i));
        }
        
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 5.0f, 5.0f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);
        player.addComponent(playerSource);
        player.addComponent(Engine::ECS::AudioListener::get());
        

        obj.init({0.0f, 20.0f, 0.0f}, {0.5f, 0.5f, 0.5f, 0.2f}, 8.0f, 8.0f);
        obj.addComponent(objRenderer);

        mainScene.objs.push_back(&player);
        mainScene.objs.push_back(&obj);
        
        updateSceneGraphics(mainScene);
        Renderer::Camera::setPosition(25, 0, 32);
    },
    [](double dt) {
        obj.pos = floor((screenToWorldCoords(IO::mousePos()) + tilemap.tilewidth / 2.0f) / tilemap.tilewidth) * tilemap.tilewidth;
        glm::vec3 cam_pos = Renderer::Camera::getPosition();
        if (Math::between(0.0f, player.pos.y, 110.0f)) {
            Renderer::Camera::setPosition(cam_pos.x, player.pos.y, cam_pos.z);
        }

        if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
            // place tile 
            addTile(tilemap, {(int) obj.pos.x / tilemap.tilewidth, (int) obj.pos.y / tilemap.tilewidth}, 15);
        }
        printf("%d\n", mainScene.objs.size());

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
