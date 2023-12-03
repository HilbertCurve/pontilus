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
#include <ecs/TextRenderer.h>
#include <graphics/Camera.h>
#include <physics2d/CollisionDetection.h>
#include <utils/PMath.h>
#include <utils/Utils.h>
#include <library/Colors.h>

#include "platformer_ext/enemy.h"

#include <string>

using namespace Pontilus;

///////////////
// Class definitions
///////////////

class Player : public Engine::ECS::GameObject {};


#define TILEMAP_WIDTH 200
#define TILEMAP_HEIGHT 300
#define NUM_TILES tilemap.size()
#define DEBUG_MODE true

///////////////
// Field declarations
///////////////

static Player player;
static Engine::ECS::SpriteRenderer playerRenderer;
static Engine::ECS::StateMachine playerController;
static Engine::ECS::AudioSource playerSource;
static Pontilus::Renderer::IconMap playerTextures;

static Enemy enemy = Enemy({0.0f, 0.0f, 0.0f});
static Engine::ECS::SpriteRenderer enemyRenderer;
static EnemyController enemyController;

static Engine::ECS::GameObject obj;
static Engine::ECS::SpriteRenderer objRenderer;
static Engine::ECS::Animation objAnimation;
static Pontilus::Renderer::IconMap tileTextures;
static Pontilus::Audio::WAVFile jump1, jump2;

static Engine::ECS::GameObject debug;
static Engine::ECS::TextRenderer debugText;
static Pontilus::Renderer::Font debugFont;
static std::string debugStuff;

static int key[TILEMAP_HEIGHT][TILEMAP_WIDTH];

///////////////
// TileMap functions
///////////////


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
    {"grounded", &playerController, [](double dt, Engine::ECS::GameObject *player) {
        // jump
        ((Engine::ECS::SpriteRenderer *)player->getComponent(typeid(Engine::ECS::SpriteRenderer)))->color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
        player->velocity.y = 0.0f;
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("jumped");
            playerController.removeState("grounded");
            player->velocity.y = 40.0f;
            Engine::ECS::AudioSource *ptr = (Engine::ECS::AudioSource *) player->getComponent(typeid(Engine::ECS::AudioSource));
            ptr->play(jump1, false);
        }
        prev = curr;

        horizontalMotion(dt);
        player->pos += glm::vec3(player->velocity * (float) dt, 0.0f);
        
        unsigned int flags = Library::collide(*player, true, tilemap);
        bool hasFloor = flags & 0b1000;
        if (!hasFloor) {
            playerController.replaceState("grounded", "jumped");
        }
    }},
    {"jumped", &playerController, [](double dt, Engine::ECS::GameObject *player) {
        ((Engine::ECS::SpriteRenderer *)player->getComponent(typeid(Engine::ECS::SpriteRenderer)))->color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
        // jump
        player->velocity -= glm::vec2{0.0f, 39.6f} * (float) dt;
        curr = Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE);
        if (curr != prev && curr) {
            playerController.addState("double-jumped");
            playerController.removeState("jumped");
            player->velocity.y = 30.0f;
            Engine::ECS::AudioSource *ptr = (Engine::ECS::AudioSource *) player->getComponent(typeid(Engine::ECS::AudioSource));
            ptr->play(jump1, false);
        }
        prev = curr;
        
        horizontalMotion(dt);
        player->pos += glm::vec3(player->velocity * (float) dt, 0.0f);

        unsigned int flags = Library::collide(*player, true, tilemap);
        bool hasFloor = flags & 0b1000;
        std::vector<Library::tile_rect> info;
        if (hasFloor && player->velocity.y <= 0.0f) {
            playerController.replaceState("jumped", "grounded");
        }
    }},
    {"double-jumped", &playerController, [](double dt, Engine::ECS::GameObject *player) {
        ((Engine::ECS::SpriteRenderer *)player->getComponent(typeid(Engine::ECS::SpriteRenderer)))->color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
        // fall
        player->velocity -= glm::vec2{0.0f, 39.6f} * (float) dt;

        horizontalMotion(dt);
        player->pos += glm::vec3(player->velocity * (float) dt, 0.0f);
        
        unsigned int flags = Library::collide(*player, true, tilemap);
        bool hasFloor = flags & 0b1000;
        if (hasFloor && player->velocity.y <= 0.0f) {
            playerController.replaceState("double-jumped", "grounded");
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

// Miniscene motion; clamp camera to location of player

void cameraClamp()
{
#ifdef DEBUG_MODE
    float speed = 2.0f;
    if (IO::isKeyPressed(GLFW_KEY_LEFT))
    {
        Renderer::Camera::move(-speed, 0.0, 0.0);
    }
    if (IO::isKeyPressed(GLFW_KEY_RIGHT))
    {
        Renderer::Camera::move(speed, 0.0, 0.0);
    }
    if (IO::isKeyPressed(GLFW_KEY_DOWN))
    {
        Renderer::Camera::move(0.0, -speed, 0.0);
    }
    if (IO::isKeyPressed(GLFW_KEY_UP))
    {
        Renderer::Camera::move(0.0, speed, 0.0);
    }
#endif
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
        Pontilus::Renderer::initIconMap("../assets/textures/ghostSwole.png", playerTextures, 675, 570, 0);
        Pontilus::Renderer::initIconMap("../assets/textures/sad_painting.png", tileTextures, 16, 16, 0);
        Pontilus::Audio::initWAVFile(jump1, "../assets/sounds/jump1.wav");
        Pontilus::Renderer::initFont(debugFont, "../assets/fonts/times.ttf", 30);

        playerRenderer.init({nullptr} /*Renderer::getTexture(playerTextures, 0)*/, glm::vec4{1.0, 1.0, 1.0, 1.0});
        playerSource.init();

        enemyRenderer.init({nullptr}, Pontilus::Colors::BLUE);

        objRenderer.init(Renderer::getTexture(tileTextures, 0), glm::vec4{0.5f, 0.5f, 0.5f, 0.2f});
        objAnimation.init(tileTextures, 0, 31, true);

        debugStuff = "";

        debugText.init(debugStuff.c_str(), debugFont, glm::vec4{1.0, 1.0, 1.0, 1.0});

        for (int i = 0; i < TILEMAP_HEIGHT * TILEMAP_WIDTH; i++) {
            (&key[0][0])[i] = -1;
        }
        key[TILEMAP_HEIGHT-1][0] = 0;
        key[TILEMAP_HEIGHT-1][7] = 0;
        key[TILEMAP_HEIGHT-1][8] = 0;

        getTileMap(TILEMAP_WIDTH, TILEMAP_HEIGHT, &key[0][0], tilemap, 4, &tileTextures);
        applyColorFilter(tilemap, {1.0f, 1.0f, 1.0f, 1.0f});
        
        playerController.init(&sControllers[0], 3);
        enemyController.create();

        player.init({0.0f, 10.0f, 0.0f}, 4.0f, 5.0f);
        player.addComponent(playerRenderer);
        player.addComponent(playerController);
        player.addComponent(playerSource);
        player.addComponent(Engine::ECS::AudioListener::get());

        enemy.init({30.0f, 10.0f, 0.0f}, 4.0, 5.0);
        enemy.addComponent(enemyRenderer);
        enemy.addComponent(enemyController);

        obj.init({0.0f, 20.0f, 0.0f}, 4.0f, 4.0f);
        obj.addComponent(objRenderer);
        obj.addComponent(objAnimation);

        debug.init(Renderer::Camera::getPosition() + glm::vec3{100.0, 10.0, 0.0}, 100.0, 12.0);
        debug.addComponent(debugText);

        // player.rotation = glm::vec3(4.0f, 4.0f, 4.0f);

        mainScene.objs.push_back(&player);
        mainScene.objs.push_back(&enemy);
        mainScene.objs.push_back(&obj);
        mainScene.objs.push_back(&debug);

        Renderer::Camera::setPosition(24, 0, 32);
    },
    [](double dt) {
        obj.pos = floor((screenToWorldCoords(IO::mousePos()) + tilemap.tilewidth / 2.0f) / tilemap.tilewidth) * tilemap.tilewidth;
    
        cameraClamp();

        glm::vec3 cam_pos = Renderer::Camera::getPosition();

        debugStuff = "Camera Position: " +
            std::to_string(cam_pos.x) + ", " +
            std::to_string(cam_pos.y) + "\n";
        debugStuff += "Placement Object Position: " +
            std::to_string(obj.pos.x) + ", " +
            std::to_string(obj.pos.y) + "\n"; 
        debugStuff += "Player Position: " +
            std::to_string(player.pos.x) + ", " +
            std::to_string(player.pos.y) + "\n";
        debugStuff += "Game Object Count: " +
            std::to_string(mainScene.objs.size()) + "\n";

        if (false && IO::isKeyPressed(GLFW_KEY_SPACE))
        {
                if (Math::between(25.0f, player.pos.x, 2*185.0f)) {
                    Renderer::Camera::setPosition(player.pos.x, cam_pos.y, cam_pos.z);
                }
                if (Math::between(0.0f, player.pos.y, 2*185.0f)) {
                    Renderer::Camera::setPosition(cam_pos.x, player.pos.y, cam_pos.z);
                }
        }

        debug.pos = Renderer::Camera::getPosition() + glm::vec3{10.0, 20.0, 0.0};
        debugText.text = debugStuff;

        if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
            addTile(tilemap, {(int) obj.pos.x / tilemap.tilewidth, (int) obj.pos.y / tilemap.tilewidth}, selectedBlock);
        } else if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_2)) {
            removeTile(tilemap, {(int) obj.pos.x / tilemap.tilewidth, (int) obj.pos.y / tilemap.tilewidth});
        }

        if (IO::isKeyPressed(GLFW_KEY_LEFT_CONTROL) &&
            IO::isKeyPressed(GLFW_KEY_S) &&
            !saved) {
            Library::serializeTileMap("./test_level.bin", tilemap);
            saved = true;
        }

        if (IO::isKeyPressed(GLFW_KEY_LEFT_CONTROL) &&
            IO::isKeyPressed(GLFW_KEY_R) &&
            !loaded) {
            Library::deserializeTileMap("./test_level.bin", tilemap);
            applyColorFilter(tilemap, {1.0f, 1.0f, 1.0f, 1.0f});
            loaded = true;
        }
        int dtile = IO::mouseScroll().y;
        selectedBlock += dtile;
        if (selectedBlock > 31) {
            selectedBlock = 0;
        }
        if (selectedBlock < 0) {
            selectedBlock = 31;
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

