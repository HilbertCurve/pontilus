#pragma once

#include <ecs/Component.h>

namespace Platformer
{
    class Player : public Pontilus::Engine::ECS::Component
    {
        // inner-class definitions, only used inside Player
        private:
        class PlayerState {
            public:
            virtual int start() = 0;
            virtual int update(double) = 0;
            virtual int stop() = 0;
        };
        class Grounded : public PlayerState {
            public:
            static Grounded &get() {
                if (!_inst)
                    _inst = new Grounded();
                return *_inst;
            }
            virtual int start();
            virtual int update(double);
            virtual int stop();
            Grounded(Grounded &) = delete;
            private:
            Grounded() {}
            static Grounded *_inst;
        };
        class Jumped : public PlayerState {
            public:
            static Jumped &get() {
                if (!_inst)
                    _inst = new Jumped();
                return *_inst;
            }
            virtual int start();
            virtual int update(double);
            virtual int stop();
            Jumped(Jumped &) = delete;
            private:
            Jumped() {}
            static Jumped *_inst;
        };
        class DoubleJumped : public PlayerState {
            public:
            static DoubleJumped &get() {
                if (!_inst)
                    _inst = new DoubleJumped();
                return *_inst;
            }
            virtual int start();
            virtual int update(double);
            virtual int stop();
            DoubleJumped(DoubleJumped &) = delete;
            private:
            DoubleJumped() {}
            static DoubleJumped *_inst;
        };

        public:
        static Player &get() {
            if (!_inst)
                _inst = new Player();
            return *_inst;
        }
        Player(Player &) = delete;
        virtual int update(double);
        int setState(PlayerState &);
        virtual bool isSingleton() { return true; }
        
        private:
        Player() : currentState(Grounded::get()), velocity(glm::vec3(0.0f)) { };
        PlayerState &currentState;
        glm::vec3 velocity;
        static Player *_inst;
    };
}
