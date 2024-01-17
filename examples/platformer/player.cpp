#include "player.h"

#include <ecs/GameObject.h>
#include <ecs/Transform.h>
#include <core/InputListener.h>
#include <renderer/SpriteRenderer.h>
#include <math/geometry/Rect.h>

namespace Platformer
{
    // Player singleton functions:

    using namespace Pontilus::Math;
    using namespace Pontilus::ECS;
    using namespace Pontilus::Renderer;
    int Player::setState(PlayerState *newState) {
        this->currentState->stop();
        this->currentState = newState;
        this->currentState->start();

        return 0;
    }
    int Player::update(double dt) {
        // A Player requires a transform component to function

        // TODO: streamlined component check function?
        Player &p = Player::get();
        GameObject &g = *p.parent;
        Transform *tptr = (Transform*)g.getComponent(typeid(Transform));
        if (!tptr) {
            __pWarning("Can only update the player if parent GameObject has a Transform!");
            return 1;
        }
        // here's how I want this player to be controlled:
        // first, the player will be a different kind of state machine. Not the standard one.
        // code for responding to user input should be in state machine code.
        // every time update is called, we defer to the update code of whatever state we're in right now.
        // 
        // At a higher level, this is a platformer. The player should jump like a platformer, slide like a platformer, etc.
        // The goal right now is to get it to move.
        this->currentState->update(dt);

        // update player's position:
        tptr->pos += p.velocity * (float) dt;
        
        return 0;
    }

    void Player::registerCollision(std::vector<TileMap::Tile> &tiles) {
        // TODO: copy code from main.cpp to here
        Transform &transform = *(Transform *) this->parent->getComponent(typeid(Transform));

        tiles.clear();

        if (!this->currentTileMap) {
            __pMessage("Skipping collision as no collider is assigned.");
            return;
        }
        TileMap &t = *this->currentTileMap;

        t.collide(transform, tiles);
    }

    bool Player::hasFloor() {
        Transform &tPlayer = *(Transform *)this->parent->getComponent(typeid(Transform));
        if (!this->currentTileMap) {
            __pMessage("Skipping collision as no collider is assigned.");
            return false;
        }

        std::vector<TileMap::Tile> tiles = std::vector<TileMap::Tile>();

        // let's make a transform, call it foot. it's a small rectangle underneath the player
        Transform playerFoot = Transform (
            glm::vec3(tPlayer.pos.x, tPlayer.pos.y - tPlayer.whd.y / 2.0f - 0.1f, 1.0f),
            glm::vec3(tPlayer.whd.x, 0.05f, 1.0f),
            glm::vec3(0.0f)
        );

        TileMap &t = *this->currentTileMap;

        t.collide(playerFoot, tiles);

        return !tiles.empty();
    }

    uint32_t Player::clip(std::vector<TileMap::Tile> &tiles) {
        Transform &transform = *(Transform *) this->parent->getComponent(typeid(Transform));

        glm::vec2 toPlayerEnd = glm::vec2(transform.whd.x, transform.whd.y) / 2.0f;
        glm::vec2 playerPos = glm::vec2(transform.pos);
        glm::vec2 toTileEnd = glm::vec2(this->currentTileMap->width(), this->currentTileMap->height()) / 2.0f;


        bool hasUp = false, hasDown = false, hasLeft = false, hasRight = false;

        for (auto tile : tiles) {
            glm::vec2 tilePos = glm::vec2(tile.pos) * toTileEnd * 2.0f;

            // let's get the horizontal and vertical overlap of this tile on our GameObject
            Rect tileRect = Rect(tilePos - toTileEnd, tilePos + toTileEnd);
            Rect playerRect = Rect(playerPos - toPlayerEnd, playerPos + toPlayerEnd);

            Rect intersection = playerRect.intersect(tileRect);

            if (intersection.width() * intersection.height() <= 0.0005f) {
                // no point in dealing with marginal intersections
                continue;
            }

            if (intersection.width() < 0.03f && intersection.height() < 0.03f) {
                // tiny corners are also passable
                continue;
            }

            if (intersection.width() < intersection.height()) {
                // we have a horizontal collision
                this->velocity.x = 0.0f;

                if (intersection.center().x < transform.pos.x) {
                    // we have an intersection to the left
                    hasLeft = true;
                    // start at center of collision, move out to edge of collision, then move out so edge of player meets edge of collision
                    transform.pos.x = tile.pos.x * toTileEnd.x * 2.0f + toTileEnd.x + transform.whd.x / 2.0f;
                    continue;
                } else {
                    // we have an intersection to the right
                    hasRight = true;
                    transform.pos.x = tile.pos.x * toTileEnd.x * 2.0f - toTileEnd.x - transform.whd.x / 2.0f;
                    continue;
                }
            } else {
                // we have a vertical collision; change velocity in special way
                if (intersection.center().y <= transform.pos.y && this->velocity.y < 0.0f) {
                    // we have a floor collision
                    hasDown = true;
                    this->velocity.y = 0.0f;
                    transform.pos.y = tile.pos.y * toTileEnd.y * 2.0f + toTileEnd.y + transform.whd.y / 2.0f;
                    continue;
                } else if (intersection.center().y > transform.pos.y && this->velocity.y >= 0.0f) {
                    // we have a ceiling collision
                    hasUp = true;
                    this->velocity.y = -0.01f;
                    transform.pos.y = tile.pos.y * toTileEnd.y * 2.0f - toTileEnd.y - transform.whd.y / 2.0f;
                    continue;
                }
            }
        }

        return (hasRight << 3) | (hasUp << 2) | (hasLeft << 1) | (hasDown << 0);
    }

    // a bunch of parameters I'll tweak with:
    static float WALK_VELOCITY = 16.0;
    static float JUMP_VELOCITY = 25.0;
    static float FRICTION = 30.0;
    static float GRAVITY = 50.0; // physics :D


    int Player::Grounded::start() {
        // while grounded, we should have no residual velocity in y direction
        Player::get().velocity.y = 0.0;

        return 0;
    }

    void Player::horizontalMovement(double dt) {
        // movement time!
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            this->velocity.x -= WALK_VELOCITY * dt;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            this->velocity.x += WALK_VELOCITY * dt;
        } else {
            if (this->velocity.x < -0.1f)
                this->velocity.x += FRICTION * dt;
            else if (this->velocity.x > 0.1f)
                this->velocity.x -= FRICTION * dt;
            else
                this->velocity.x = 0.0f;
        }
    }

    int Player::Grounded::update(double dt) {
        using namespace Pontilus::ECS;
        // we can assume with confidence that Player's parent has a transform, as this condition was checked in Player::update()
        Player &p = Player::get();
        __pMessage("Grounded");

        p.horizontalMovement(dt);
        std::vector<TileMap::Tile> tiles;
        p.registerCollision(tiles);

        // temporary debug thing
        SpriteRenderer *pspr = (SpriteRenderer *) p.parent->getComponent(typeid(SpriteRenderer));
        if (tiles.empty()) {
            pspr->color = {0.0, 0.0, 1.0, 1.0};
        } else {
            pspr->color = {0.0, 1.0, 0.0, 1.0};
        }

        p.clip(tiles);

        if (Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE)) {
            p.velocity.y = JUMP_VELOCITY;
        }

        if (!(p.hasFloor())) {
            p.setState(Jumped::get());
            return 0;
        }


        return 0;
    }

    int Player::Grounded::stop() {
        // meh, do nothing for now.
        return 0;
    }

    
    int Player::Jumped::start() {
        //Player::get().velocity.y = JUMP_VELOCITY;
        return 0;
    }

    // mate falls due to gravity; that's where the `double dt` comes to play; acceleration
    int Player::Jumped::update(double dt) {
        using namespace Pontilus::ECS;
        Player &p = Player::get();

        __pMessage("Jumped");

        p.velocity.y -= GRAVITY * dt;
        p.horizontalMovement(dt);
        std::vector<TileMap::Tile> tiles;
        p.registerCollision(tiles);

        // temporary debug thing
        SpriteRenderer *pspr = (SpriteRenderer *) p.parent->getComponent(typeid(SpriteRenderer));
        if (tiles.empty()) {
            pspr->color = {0.0, 0.0, 1.0, 1.0};
        } else {
            pspr->color = {0.0, 1.0, 0.0, 1.0};
        }

        uint32_t dirs = p.clip(tiles);

        __pMessage("%u", dirs);

        if (p.hasFloor()) {
            p.setState(Grounded::get());
            return 0;
        }

        return 0;
    }

    int Player::Jumped::stop() {
        return 0;
    }
    int Player::DoubleJumped::start() { return 0; }
    int Player::DoubleJumped::update(double) { return 0; }
    int Player::DoubleJumped::stop() { return 0; }

    // initialize instances???

    Player *Player::_inst = nullptr;
    Player::Grounded *Player::Grounded::g_inst = nullptr;
    Player::Jumped *Player::Jumped::j_inst = nullptr;
    Player::DoubleJumped *Player::DoubleJumped::dj_inst = nullptr;
}
