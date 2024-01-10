#include "player.h"

#include <ecs/GameObject.h>
#include <ecs/Transform.h>
#include <core/InputListener.h>

namespace Platformer
{
    // Player singleton functions:

    int Player::setState(PlayerState &newState) {
        this->currentState.stop();
        this->currentState = newState;
        this->currentState.start();

        return 0;
    }
    int Player::update(double dt) {
        // A Player requires a transform component to function

        // TODO: streamlined component check function?
        using namespace Pontilus::Engine::ECS;
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
        this->currentState.update(dt);

        // update player's position:
        tptr->pos += p.velocity * (float) dt;
        
        return 0;
    }

    // Behavior while grounded: ground considered y=0;

    int Player::Grounded::start() {
        using namespace Pontilus::Engine::ECS;
        // while grounded, we should have no residual velocity in y direction
        Player::get().velocity.y = 0.0;

        return 0;
    }

    int Player::Grounded::update(double dt) {
        using namespace Pontilus::Engine::ECS;
        // we can assume with confidence that Player's parent has a transform, as this condition was checked in Player::update()
        Player &p = Player::get();

        // movement time!
        if (Pontilus::IO::isKeyPressed(GLFW_KEY_A)) {
            p.velocity.x = -16.0;
        } else if (Pontilus::IO::isKeyPressed(GLFW_KEY_D)) {
            p.velocity.x = 16.0;
        } else {
            p.velocity.x = 0.0;
        }

        return 0;
    }

    int Player::Grounded::stop() {
        // meh, do nothing for now.
        return 0;
    }

    // the rest are big TODOS
    int Player::Jumped::start() { return 0; }
    int Player::Jumped::update(double) { return 0; }
    int Player::Jumped::stop() { return 0; }
    int Player::DoubleJumped::start() { return 0; }
    int Player::DoubleJumped::update(double) { return 0; }
    int Player::DoubleJumped::stop() { return 0; }

    // initialize instances???

    Player *Player::_inst = nullptr;
    Player::Grounded *Player::Grounded::_inst = nullptr;
    Player::Jumped *Player::Jumped::_inst = nullptr;
    Player::DoubleJumped *Player::DoubleJumped::_inst = nullptr;
}