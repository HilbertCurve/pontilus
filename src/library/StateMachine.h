/* date = 13 February 2022 4:48 pm */

#pragma once

#include <vector>

#include "ecs/Component.h"

namespace Pontilus
{
    namespace Library
    {
        class StateMachine;

        typedef void (*_update)(double dt, ECS::Entity *parent);
        class State
        {
            public:
            const char *name;
            StateMachine *parent;
            _update callback;

            void init(const char *name, _update callback);
        };

        class StateMachine : public ECS::Component
        {
            public:
            std::vector<State *> states;
            int count;
            std::vector<bool> currentStates;

            StateMachine() = default;
            StateMachine(State* states, int count) { this->init(states, count); }
            void init(State *states, int count);

            static void updateAll(double dt);

            virtual int update(double dt);
            int addState(State *state);
            int addState(const char *state);
            int removeState(State *state);
            int removeState(const char *state);
            int replaceState(State *state1, State *state2);
            int replaceState(const char *state1, const char *state2);
            int xAddState(State *state);
            int xAddState(const char *state);
            int xRemoveState(State *state);
            int xRemoveState(const char *state);
            bool hasState(State *state);
            bool hasState(const char *state);

            bool operator==(StateMachine other);
        };
    }
}