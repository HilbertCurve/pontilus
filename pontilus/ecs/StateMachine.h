/* date = 13 February 2022 4:48 pm */

#pragma once

#include <vector>

#include "ecs/Component.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            class StateMachine;

            typedef void (*_update)(double dt);
            class State
            {
                public:
                const char *name;
                StateMachine *parent;
                _update callback;

                void init(const char *name, _update callback);
            };

            class StateMachine : public Component
            {
                public:
                std::vector<State *> states;
                int count;
                State *currentState;

                void init(State *states, int count);

                static void updateAll(double dt);

                void update(double dt);
                int setState(State *state);
                int setState(const char *state);

                bool operator==(StateMachine other);
            };
        }
    }
}