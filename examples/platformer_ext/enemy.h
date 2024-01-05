#pragma once

#include "common.h"
#include <vector>

class Enemy : public Pontilus::Engine::ECS::GameObject 
{
    public:
    Enemy(glm::vec3 _pos) {this->init(_pos, 4.0f, 5.0f);};
};

class EnemyController : public Pontilus::Engine::ECS::StateMachine
{
    private:
    Pontilus::Engine::ECS::State states[1];
    public:
    EnemyController() { this->create(); }
    void create()
    {
        Pontilus::Engine::ECS::State eControllers[] = {
            {"idle", this, [](double dt, Pontilus::Engine::ECS::GameObject *_parent) {
                _parent->velocity -= glm::vec2{0.0f, 39.6f} * (float) dt;

                unsigned int flags = Pontilus::Library::collide(*_parent, true, tilemap);
                bool hasFloor = flags & 0b1000;
                std::vector<Pontilus::Library::tile_rect> info;
                if (hasFloor && _parent->velocity.y <= 0.0f) {
                    _parent->velocity.y = 0.0f;
                }
                _parent->pos += glm::vec3(_parent->velocity * (float) dt, 0.0f);
            }}
        };

        this->states[0] = eControllers[0];
        this->init(&this->states[0], 1);
        this->addState(&states[0]);
    }
};