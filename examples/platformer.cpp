#include <stdio.h>
#include <audio/AudioMaster.h>
#include <core/Application.h>
#include <core/Scene.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/Component.h>
#include <ecs/StateMachine.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/Animation.h>
#include <ecs/Body2D.h>
#include <ecs/AudioListener.h>
#include <ecs/AudioSource.h>
#include <graphics/Camera.h>
#include <physics2d/CollisionDetection.h>
#include <utils/PMath.h>
#include <utils/Utils.h>

#include "library/TileMap.h"

using namespace Pontilus;

///////////////
// Class definitions
///////////////

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity;
};

#define TILEMAP_WIDTH 30
#define TILEMAP_HEIGHT 30
#define NUM_TILES tilemap.size()

///////////////
// Field declarations
///////////////

static Player player;
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::StateMachine playerController;
static Engine::ECS::AudioSource playerSource;
static Pontilus::Graphics::IconMap playerTextures;
static Engine::ECS::GameObject obj;
static Engine::ECS::SpriteRenderer objRenderer;
static Engine::ECS::Animation objAnimation;
static Pontilus::Graphics::IconMap tileTextures;
static Pontilus::Audio::WAVFile jump1, jump2;

static int key[TILEMAP_HEIGHT][TILEMAP_WIDTH];

///////////////
// TileMap functions
///////////////

Library::TileMap tilemap = Library::TileMap();

static void collide(double dt) {
    // collision
    std::vector<Library::tile_rect> info;
    Library::getCollisionInfo(player, info, tilemap);
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

static bool checkForFloor(std::vector<Library::tile_rect> &t) {
    bool ret = false;
    t.clear();
    for (int i = 0; i < tilemap.size(); i++) {
        Library::rect foot = {
            {player.pos.x - player.width / 2.0f + 0.1f, player.pos.y - player.height / 2.0f - 0.1f},
            {player.pos.x + player.width / 2.0f - 0.1f, player.pos.y - player.height / 2.0f - 0.1f},
        };
        if (Library::detectRectRect(foot, Library::rectFromObj(tilemap[i]))) {
            ret = true;
            t.push_back({tilemap[i], {}});
            break;
        }
    }

    return ret;
}

///////////////
// Player State Machine
///////////////

static void horizontalMotion(double dt) {
    // move
    if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
        player.velocity.x -= 150.0f * dt;
        if (player.velocity.x < -32.0f) player.velocity.x = -32.0f;
    } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
        player.velocity.x += 150.0f * dt;
        if (player.velocity.x > 32.0f) player.velocity.x = 32.0f;
    } else {
        if (player.velocity.x < -1.0f) player.velocity.x += 150.0f * dt;
        else if (player.velocity.x > 1.0f) player.velocity.x -= 150.0f * dt;
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
        std::vector<Library::tile_rect> info;
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
        std::vector<Library::tile_rect> info;
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
        std::vector<Library::tile_rect> info;
        bool hasFloor = checkForFloor(info);
        if (hasFloor && player.velocity.y <= 0.0f) {
            playerController.replaceState("double-jumped", "grounded");
            //Tile floor = info.at(0).first;
            //player.pos.y = floor.pos.y + (floor.height - player.height) / 2.0f;
        }
    }},
};

///////////////////////////
// Serialization and Deserialization
///////////////////////////

void serialize(const char *filepath) {
    FILE *fp;
    fp = fopen(filepath, "w");
    if (!fp) {
        __pWarning("Opening file %s failed.", filepath);
        return;
    }

    fputc(TILEMAP_WIDTH, fp);
    fputc(TILEMAP_HEIGHT, fp);
    fputc('\n', fp);

    for (int i = 0; i < TILEMAP_WIDTH * TILEMAP_HEIGHT; i++) {
        fputc((&key[0][0])[i], fp);
    }

    fclose(fp);
}

void deserialize(const char *filepath, Library::TileMap &t) {
    FILE *fp;
    fp = fopen(filepath, "r");
    if (!fp) {
        __pWarning("Opening file %s failed.", filepath);
        return;
    }

    int width = fgetc(fp);
    int height = fgetc(fp);
    fseek(fp, 1, SEEK_CUR);
    int m_width = fmin(width, t.width);
    int m_height = fmin(height, t.height);

    memset(t.key, 0, t.width * t.height);

    for (int j = 0; j < m_height; j++) {
        for (int i = 0; i < m_width; i++) {
            t.key[i + width * j] = (signed char) fgetc(fp);
        }
        if (m_width < t.width) {
            fseek(fp, t.width - m_width, SEEK_CUR);
        }
    }

    Library::deleteTileMap(t);
    Library::getTileMap(t.width, t.height, t.key, t, t.tilewidth, t.tileset);
}

///////////////
// Scene declaration
///////////////

float selectedBlock = 0;
const float maxSelectedBlock = 30;

bool saved = false;
bool loaded = false;

static Engine::Scene mainScene = {
    []() {
        Pontilus::Graphics::initIconMap("./assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Graphics::initIconMap("./assets/textures/tilemap1.png", tileTextures, 16, 16, 0);
        Pontilus::Audio::initWAVFile(jump1, "./assets/sounds/jump1.wav");

        playerRenderer.init({nullptr}/*Graphics::getTexture(playerTextures, 0)*/);
        playerSource.init();
        objRenderer.init(Graphics::getTexture(tileTextures, 0));
        objAnimation.init(tileTextures, 0, 30, true);

        for (int i = 0; i < TILEMAP_HEIGHT * TILEMAP_WIDTH; i++) {
            (&key[0][0])[i] = -1;
        }
        key[29][0] = 0;

        getTileMap(TILEMAP_WIDTH, TILEMAP_HEIGHT, &key[0][0], tilemap, 8, &tileTextures);
        applyColorFilter(tilemap, {1.0f, 1.0f, 1.0f, 1.0f});
        
        playerController.init(&sControllers[0], 3);

        player.init({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 5.0f, 5.0f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);
        player.addComponent(playerSource);
        player.addComponent(Engine::ECS::AudioListener::get());

        obj.init({0.0f, 20.0f, 0.0f}, {0.5f, 0.5f, 0.5f, 0.2f}, 8.0f, 8.0f);
        obj.addComponent(objRenderer);
        obj.addComponent(objAnimation);

        mainScene.objs.push_back(&player);
        mainScene.objs.push_back(&obj);
        
        updateSceneGraphics(mainScene);
        Renderer::Camera::setPosition(25, 0, 32);
    },
    [](double dt) {
        obj.pos = floor((screenToWorldCoords(IO::mousePos()) + tilemap.tilewidth / 2.0f) / tilemap.tilewidth) * tilemap.tilewidth;
        glm::vec3 cam_pos = Renderer::Camera::getPosition();
        float cam_x = cam_pos.x;
        float cam_y = cam_pos.y;
        if (Math::between(25.0f, player.pos.x, 185.0f)) {
            Renderer::Camera::setPosition(player.pos.x, cam_pos.y, cam_pos.z);
            cam_pos.x = player.pos.x;
        }
        if (Math::between(0.0f, player.pos.y, 185.0f)) {
            Renderer::Camera::setPosition(cam_pos.x, player.pos.y, cam_pos.z);
            cam_pos.y = player.pos.y;
        }

        if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
            addTile(tilemap, {(int) obj.pos.x / tilemap.tilewidth, (int) obj.pos.y / tilemap.tilewidth}, selectedBlock);
        } else if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_2)) {
            removeTile(tilemap, {(int) obj.pos.x / tilemap.tilewidth, (int) obj.pos.y / tilemap.tilewidth});
        }

        if (IO::isKeyPressed(GLFW_KEY_LEFT_CONTROL) &&
            IO::isKeyPressed(GLFW_KEY_S) &&
            !saved) {
            serialize("./bin/test_level.bin");
            saved = true;
        }

        if (IO::isKeyPressed(GLFW_KEY_LEFT_CONTROL) &&
            IO::isKeyPressed(GLFW_KEY_R) &&
            !loaded) {
            deserialize("./bin/test_level.bin", tilemap);
            applyColorFilter(tilemap, {1.0f, 1.0f, 1.0f, 1.0f});
            loaded = true;
        }
        int dtile = IO::mouseScroll().y;
        selectedBlock += IO::mouseScroll().y;
        if (selectedBlock > 30) {
            selectedBlock = 0;
        }
        if (selectedBlock < 0) {
            selectedBlock = 30;
        }

        if (dtile > 0) {
            while (dtile) {
                objAnimation.next();
                dtile--;
            }
        } else if (dtile < 0) {
            while (dtile) {
                objAnimation.previous();
                dtile++;
            }
        }

        updateSceneGraphics(mainScene);
    },
    []() {

    }
};

///////////////
// Library initialization
///////////////

int main() 
{
    Pontilus::setEcho(true);
    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();
}
