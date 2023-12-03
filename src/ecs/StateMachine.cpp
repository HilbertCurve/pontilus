#include "ecs/StateMachine.h"
#include "core/InputListener.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {

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
                    this->currentStates.push_back(false);
                    states[i].parent = this;
                }

                this->addState(&states[0]);
            }

            int StateMachine::update(double dt)
            {
                for (int i = 0; i < states.size(); i++)
                {
                    if (currentStates[i]) states[i]->callback(dt, this->parent);
                }

                return 0;
            }

            int StateMachine::addState(State *state)
            {
                if (!state)
                {
                    __pWarning("Cannot add null state to a StateMachine.");
                    return 0;
                }

                bool isIn = false;
                int loc;
                for (int i = 0; i < this->count; i++)
                {
                    State *s = this->states.at(i);
                    if (s == state) {
                        isIn = true;
                        loc = i;
                        break;
                    }
                }
                
                if (isIn)
                    this->currentStates[loc] = true;
                else
                {
                    if (state->name) {
                        __pWarning("Attempt at adding illegal state %s to StateMachine %p.", state->name, this);
                    } else {
                        __pWarning("Attempt at adding illegal anonymous state %p to StateMachine %p.", state, this);
                    }
                    return 0;
                }

                return 1;
            }

            int StateMachine::addState(const char *state)
            {
                bool isIn = false;
                int loc;
                State *s;
                for (int i = 0; i < this->count; i++)
                {
                    s = this->states.at(i);
                    if (s->name && strcmp(this->states.at(i)->name, state) == 0)
                    {
                        isIn = true;
                        loc = i;
                        break;
                    }
                }
                if (isIn)
                {
                    this->currentStates[loc] = true;
                    return 1;
                }
                else
                {
                    __pWarning("Attempt at adding illegal state %s to StateMachine %p.", state, this);
                    return 0;
                }
            }

            int StateMachine::removeState(State *state)
            {
                bool isIn = false;
                int loc = 0;
                for (int i = 0; i < this->count; i++)
                {
                    State *s = this->states.at(i);
                    if (s == state) {
                        isIn = true;
                        loc = i;
                        break;
                    }
                }
                if (isIn)
                {
                    this->currentStates[loc] = false;
                    return 1;
                }
                else
                {
                    if (state->name)
                    {
                        __pWarning("Attempt at removing state %s from StateMachine %p.", state->name, this);
                        return 0;
                    }
                    else
                    {
                        __pWarning("Attempt at removing anonymous state %p from StateMachine %p.", state, this);
                        return 0;
                    }
                }
            }

            int StateMachine::removeState(const char *state)
            {
                bool isIn = false;
                int loc = 0;
                State *s;
                for (int i = 0; i < this->count; i++)
                {
                    s = this->states.at(i);
                    if (s->name && strcmp(this->states.at(i)->name, state) == 0) {
                        isIn = true;
                        loc = i;
                        break;
                    }
                }
                if (isIn)
                {
                    this->currentStates[loc] = false;
                    return 1;
                }
                else
                {
                    __pWarning("Attempt at removing illegal state %s from StateMachine %p.", state, this);
                    return 0;
                }
            }

            int StateMachine::replaceState(State *state1, State *state2)
            {
                int a = StateMachine::xRemoveState(state1);
                int b = StateMachine::addState(state2);
                if (!a || !b) return 0;
                else return 1;
            }

            int StateMachine::replaceState(const char *state1, const char *state2)
            {
                int a = StateMachine::xRemoveState(state1);
                int b = StateMachine::addState(state2);
                if (!a || !b) return 0;
                else return 1;
            }

            bool StateMachine::hasState(State *state)
            {
                bool ret = false;
                for (int i = 0; i < this->states.size(); i++)
                {
                    if (currentStates[i])
                    {
                        if (states[i] == state)
                            ret = true;
                    }
                }
                return ret;
            }

            bool StateMachine::hasState(const char *state)
            {
                bool ret = false;
                for (int i = 0; i < this->states.size(); i++)
                {
                    if (currentStates[i])
                    {
                        if (states[i]->name && strcmp(states[i]->name, state) == 0)
                            ret = true;
                    }
                }
                return ret;
            }

            int StateMachine::xAddState(State *state)
            {
                if (!this->hasState(state))
                    return this->addState(state);
                
                return 1;
            }

            int StateMachine::xAddState(const char *state)
            {
                if (!this->hasState(state))
                    return this->addState(state);
                
                return 1;
            }

            int StateMachine::xRemoveState(State *state)
            {
                if (this->hasState(state))
                    return this->removeState(state);
                
                return 1;
            }

            int StateMachine::xRemoveState(const char *state)
            {
                if (this->hasState(state))
                    return this->removeState(state);
                
                return 1;
            }
        }
    }
}