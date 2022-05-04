#include <core/Application.h>
#include <core/InputListener.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/StateMachine.h>
#include <ecs/TextRenderer.h>
#include <library/TileMap.h>
#include <graphics/Camera.h>
#include <utils/PMath.h>

using namespace Pontilus;

class Player : public Engine::ECS::GameObject {
    public:
    glm::vec2 velocity;
};

class TextBox : public Engine::ECS::GameObject {
    public:
    TextBox() {
        this->text = "";
    }
    void setText(const char *text, Renderer::Font &font) {
        this->text = text;

        this->r_text.init("", font);

        this->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
        this->r_box.init({nullptr});

        this->addComponent(this->r_text);
        this->addComponent(this->r_box);
    }
    void appear() {
        this->r_text.text = std::string(text);
        this->color = {0.2f, 0.2f, 0.2f, 1.0f};
        this->visible = true;
    }
    void disappear() {
        this->r_text.text = std::string("");
        this->color = {0.0f, 0.0f, 0.0f, 0.0f};
        this->visible = false;
    }
    Engine::ECS::TextRenderer r_text;
    private:
    Engine::ECS::SpriteRenderer r_box;
    std::string text = "";
    bool visible = false;
};

static Renderer::Font timesNewRoman;

class NPC : public Engine::ECS::GameObject {
    public:
    void setText() {
        if (!timesNewRoman.filepath)
            Renderer::initFont(timesNewRoman, "assets/fonts/times.ttf", 26);

        if (!added) getCurrentScene()->addObj(&this->myBox);
        this->myBox.init({this->pos.x, this->pos.y + 2.0f, 0.0f},
                {1.0f, 1.0f, 1.0f, 0.0f}, 30, 10);
        this->myBox.setText("", timesNewRoman);
    }
    void setText(const char *dialog) {
        if (!timesNewRoman.filepath)
            Renderer::initFont(timesNewRoman, "assets/fonts/times.ttf", 26);

        if (!added) getCurrentScene()->addObj(&this->myBox);
        this->myBox.init({this->pos.x, this->pos.y + 12.0f, 0.0f},
                {1.0f, 1.0f, 1.0f, 0.0f}, 30, 30);
        this->myBox.setText(dialog, timesNewRoman);
    }
    void talk() {
        this->myBox.appear();
    }
    void hush() {
        this->myBox.disappear();
    }
    private:
    TextBox myBox;
    bool added = 0;
};

static Player player;
static Engine::ECS::SpriteRenderer r_player;
static Engine::ECS::StateMachine c_stepwise;
static Engine::ECS::StateMachine c_continuous;
static NPC npc;
static Engine::ECS::SpriteRenderer r_npc;
static Renderer::IconMap npc_icons;
static TextBox tb;
static Library::TileMap tilemap;
static Renderer::IconMap tilemap_icons;

enum direction {
    NORTH = 0b0001,
    EAST = 0b0010,
    SOUTH = 0b0100,
    WEST = 0b1000,
    NONE = 0b0000
};

#define TILEMAP_WIDTH 4
#define TILEMAP_HEIGHT 4
#define PLAYER_SPEED 10

int key[TILEMAP_HEIGHT][TILEMAP_WIDTH];

void move(direction dir, double dt) {
    static double t_acc;
    t_acc += dt;
    if (t_acc > 0.2) t_acc = 0.0;
}

static int dir;
static Engine::ECS::State stepwise[] = {
    {"still", &c_stepwise, [](double dt) 
        {
            dir = NONE;
            if (IO::isKeyPressed(GLFW_KEY_W)) {
                dir |= NORTH;
                c_stepwise.replaceState("still", "moving");
            }
            if (IO::isKeyPressed(GLFW_KEY_S)) {
                dir |= SOUTH;
                c_stepwise.replaceState("still", "moving");
            }
            if (IO::isKeyPressed(GLFW_KEY_A)) {
                dir |= WEST;
                c_stepwise.replaceState("still", "moving");
            }
            if (IO::isKeyPressed(GLFW_KEY_D)) {
                dir |= EAST;
                c_stepwise.replaceState("still", "moving");
            }
        }},
    {"moving", &c_stepwise, [](double dt) 
        {
            static double t_acc = 0.0;
            t_acc += dt;
            if (t_acc > 0.2) {
                c_stepwise.replaceState("moving", "still");
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
        }}
};

static Library::rect playerFaceExtent(Engine::ECS::GameObject p, direction d) {
    using namespace Pontilus::Library;
    rect ret;
    switch (d) {
        case NORTH:
            ret = {glm::vec2(p.pos) + glm::vec2{-p.width/2, p.height/2 - 0.1f}, 
                glm::vec2(p.pos) + glm::vec2{p.width/2, p.height/2}}; break;
        case SOUTH:
            ret = {glm::vec2(p.pos) + glm::vec2{-p.width/2, -p.height/2}, 
                glm::vec2(p.pos) + glm::vec2{p.width/2, -p.height/2 + 0.1f}}; break;
        case EAST:
            ret = {glm::vec2(p.pos) + glm::vec2{p.width/2 - 0.1f, -p.height/2}, 
                glm::vec2(p.pos) + glm::vec2{p.width/2, p.height/2}}; break;
        case WEST:
            ret = {glm::vec2(p.pos) + glm::vec2{-p.width/2, p.height/2},
                glm::vec2(p.pos) + glm::vec2{-p.width/2 + 0.1f, p.height/2}}; break;
        default:
            ret = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)}; break;
    };
    return ret;
}

static Engine::ECS::State continuous[] = {
    {"sentinel", &c_continuous, [](double dt)
        {
            using namespace Pontilus::Library;

            CollisionInfo info;
            glm::vec2 snapPos = glm::vec2(0.0f, 0.0f);

            player.width += 0.1f;
            player.height += 0.1;
            getCollisionInfo(player, info, tilemap);
            player.width -= 0.1f;
            player.height -= 0.1f;

            int collidingFaces = NONE;
            for (auto collision : info) {
                // if collision box's width is longer than it's height, collide from top-down
                if (collision.second.max.x - collision.second.min.x >
                        collision.second.max.y - collision.second.min.y) {
                    if (collision.first.pos.y > player.pos.y) {
                        collidingFaces |= (int) NORTH;
                        snapPos.y = glm::min(snapPos.y, collision.first.pos.y - collision.first.height / 2);
                    } else {
                        collidingFaces |= (int) SOUTH;
                        snapPos.y = glm::max(snapPos.y, collision.first.pos.y + collision.first.height / 2);
                    }
                } else {
                    if (collision.first.pos.x > player.pos.x) {
                        collidingFaces |= (int) EAST;
                        snapPos.x = glm::min(snapPos.x, collision.first.pos.x - collision.first.width / 2);
                    } else {
                        collidingFaces |= (int) WEST;
                        snapPos.x = glm::max(snapPos.x, collision.first.pos.x + collision.first.width / 2);
                    }
                }
            }

            // if player is colliding on parallel sides, set snap to player's position
            if (collidingFaces & (NORTH | SOUTH) == (NORTH | SOUTH)) {
                snapPos.y = player.pos.y;
            }
            if (collidingFaces & (EAST | WEST) == (EAST | WEST)) {
                snapPos.x = player.pos.x;
            }
            

            player.velocity = glm::vec2(0.0f, 0.0f);
            if (IO::isKeyPressed(GLFW_KEY_W) && !(collidingFaces & NORTH)) {
                player.velocity += glm::vec2(0.0f, 1.0f);
            }
            if (IO::isKeyPressed(GLFW_KEY_S) && !(collidingFaces & SOUTH)) {
                player.velocity += glm::vec2(0.0f, -1.0f);
            }
            if (IO::isKeyPressed(GLFW_KEY_A) && !(collidingFaces & WEST)) {
                player.velocity += glm::vec2(-1.0f, 0.0f);
            }
            if (IO::isKeyPressed(GLFW_KEY_D) && !(collidingFaces & EAST)) {
                player.velocity += glm::vec2(1.0f, 0.0f);
            }
            player.velocity *= 10.f;
            player.pos += glm::vec3(player.velocity * (float) dt, 0.0f);
        }}
};

bool hidden = false;

Engine::Scene mainScene = {
    []() {
        player = Player();
        player.init({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
        npc.init({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 4, 4);
        npc.setText("Greetings!");

        // tilemap
        Renderer::initIconMap("./assets/textures/tilemap1.png", tilemap_icons, 16, 16, 0);
        Renderer::initIconMap("./assets/textures/ghostSwole.png", npc_icons, 675, 570, 0);

        for (int i = 0; i < TILEMAP_HEIGHT * TILEMAP_WIDTH; i++) {
            (&(key[0][0]))[i] = -1;
        }
        key[0][3] = 0;

        Library::getTileMap(TILEMAP_WIDTH, TILEMAP_HEIGHT, &key[0][0], tilemap, 4, &tilemap_icons);

        r_player.init(getTexture(tilemap_icons, 0));
        c_continuous.init(&continuous[0], 1);

        r_npc.init(getTexture(npc_icons, 0));

        player.addComponent(r_player);
        player.addComponent(c_continuous);
        npc.addComponent(r_npc);
        npc.talk();

        mainScene.addObj(&player);
        mainScene.addObj(&npc);

        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        if (IO::isKeyPressed(GLFW_KEY_R)) {
            if (IO::isKeyPressed(GLFW_KEY_Z)) {
                player.rotation += 10.0f * dt;
            } else {
                player.rotation.y += 1.0f * dt;
            }
        }
        if (IO::isKeyPressed(GLFW_KEY_H)) {
            if (!hidden) {
                npc.hush();
                hidden = true;
            }
        }

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

