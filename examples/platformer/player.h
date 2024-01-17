/* date = January 10th 2024 */

#pragma once

#include <ecs/Component.h>

#include "tilemap.h"

namespace Platformer
{
    class Player : public Pontilus::ECS::Component
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
            static Grounded *get() {
                if (!g_inst)
                    g_inst = new Grounded();
                return g_inst;
            }
            virtual int start();
            virtual int update(double);
            virtual int stop();
            Grounded(Grounded &) = delete;
            private:
            Grounded() {}
            static Grounded *g_inst;
        };
        class Jumped : public PlayerState {
            public:
            static Jumped *get() {
                if (!j_inst)
                    j_inst = new Jumped();
                return j_inst;
            }
            virtual int start();
            virtual int update(double);
            virtual int stop();
            Jumped(Jumped &) = delete;
            private:
            Jumped() {}
            static Jumped *j_inst;
        };
        class DoubleJumped : public PlayerState {
            public:
            static DoubleJumped *get() {
                if (!dj_inst)
                    dj_inst = new DoubleJumped();
                return dj_inst;
            }
            virtual int start();
            virtual int update(double);
            virtual int stop();
            DoubleJumped(DoubleJumped &) = delete;
            private:
            DoubleJumped() {}
            static DoubleJumped *dj_inst;
        };

        public:
        static Player &get() {
            if (!_inst)
                _inst = new Player();
            return *_inst;
        }
        Player(Player &) = delete;
        virtual int update(double);
        glm::vec3 &getVelocity() { return this->velocity; }
        int setState(PlayerState *);
        virtual bool isSingleton() { return true; }
        void collideWith(TileMap &t) { this->currentTileMap = &t; }
        
        private:
        void horizontalMovement(double dt);
        Player() : currentState(Grounded::get()), velocity(glm::vec3(0.0f)) { };
        void registerCollision(std::vector<TileMap::Tile> &tiles);
        uint32_t clip(std::vector<TileMap::Tile> &tiles);
        bool hasFloor();
        PlayerState *currentState;
        TileMap *currentTileMap;
        glm::vec3 velocity;
        static Player *_inst;
    }; // class Player
}
