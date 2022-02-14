#include "ecs/StateMachine.h"
#include "core/InputListener.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            static std::vector<StateMachine *> stateMachines;

            void State::init(const char *name, _update callback)
            {
                this->name = name;
                this->callback = callback;
            }

            // this function is necessary if you want your state machine to be updated.
            // state is always set to first state listed.
            void StateMachine::init(State *states, int count)
            {
                this->count = count;

                for (int i = 0; i < count; i++)
                {
                    this->states.push_back(&states[i]);
                    states[i].parent = this;
                }

                this->setState(&states[0]);

                stateMachines.push_back(this);
            }

            void StateMachine::update(double dt)
            {
                if (this->currentState) this->currentState->callback(dt);
            }

            int StateMachine::setState(State *state)
            {
                bool isIn = false;
                for (int i = 0; i < this->count; i++)
                {
                    if (this->states.at(i) == state) {
                        isIn = true;
                        break;
                    }
                }
                if (isIn)
                    this->currentState = state;
                else
                {
                    if (state->name) {
                        __pWarning("Attempt at setting StateMachine %p's state to state %s.", this, state->name);
                    } else {
                        __pWarning("Attempt at setting StateMachine %p's state to anonymous state.", this);
                    }
                    return 0;
                }

                return 1;
            }

            int StateMachine::setState(const char *state)
            {
                bool isIn = false;
                State *s;
                for (int i = 0; i < this->count; i++)
                {
                    if (strcmp(this->states.at(i)->name, state) == 0)
                    {
                        isIn = true;
                        s = this->states.at(i);
                        break;
                    }
                }
                if (isIn)
                {
                    this->currentState = s;
                    return 1;
                }
                else
                {
                    __pWarning("Attempt at setting StateMachine %p's state to state %s.", this, state);
                    return 0;
                }
            }

            void StateMachine::updateAll(double dt)
            {
                for (StateMachine *sm : stateMachines) {
                    sm->update(dt);
                }
            }

            namespace StateMachines
            {
                
            }
        }
    }
}