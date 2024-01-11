#include "player.h"

#include <ecs/GameObject.h>
#include <ecs/Transform.h>
#include <core/InputListener.h>

namespace Platformer
{
    // Player singleton functions:

    using namespace Pontilus::Engine::ECS;
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

    // a bunch of parameters I'll tweak with:
    static float WALK_VELOCITY = 16.0;
    static float JUMP_VELOCITY = 25.0;
    static float GRAVITY = 50.0; // physics :D
    static float GROUND_LEVEL = -12.0;

    // Behavior while grounded: ground considered y=0;

    int Player::Grounded::start() {
        // while grounded, we should have no residual velocity in y direction
        Player::get().velocity.y = 0.0;
        Player &p = Player::get();
        GameObject &g = *p.parent;
        Transform *tptr = (Transform*)g.getComponent(typeid(Transform));
        tptr->pos.y = GROUND_LEVEL;

        return 0;
    }

    void Player::horizontalMovement() {
        // movement time!
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            this->velocity.x = -WALK_VELOCITY;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            this->velocity.x = WALK_VELOCITY;
        } else {
            this->velocity.x = 0.0;
        }
    }

    int Player::Grounded::update(double) {
        using namespace Pontilus::Engine::ECS;
        // we can assume with confidence that Player's parent has a transform, as this condition was checked in Player::update()
        Player &p = Player::get();
        GameObject &g = *p.parent;
        Transform *tptr = (Transform*)g.getComponent(typeid(Transform));

        if (tptr->pos.y > GROUND_LEVEL) {
            p.setState(Jumped::get());
            return 0;
        }

        p.horizontalMovement();

        // this is how we jump: we press space, set state to Jumped, then return
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_SPACE)) {
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
        Player::get().velocity.y = JUMP_VELOCITY;
        return 0;
    }

    // mate falls due to gravity; that's where the `double dt` comes to play; acceleration
    int Player::Jumped::update(double dt) {
        using namespace Pontilus::Engine::ECS;
        Player &p = Player::get();
        GameObject &o = *p.parent;
        Transform *t = (Transform *)o.getComponent(typeid(Transform));

        p.velocity.y -= GRAVITY * dt;
        p.horizontalMovement();

        if (t->pos.y + p.velocity.y * dt <= GROUND_LEVEL && p.velocity.y < 0.0) {
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
