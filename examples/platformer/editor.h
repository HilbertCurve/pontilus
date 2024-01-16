/* date = January 16 2024 8:40 am */

#pragma once

#include <ecs/Component.h>
#include "tilemap.h"

namespace Platformer
{
    class Editor : public Pontilus::ECS::Component
    {
        public:
        virtual int update(double);
        static Editor &get() {
            if (!_inst)
                _inst = new Editor();
            return *_inst;
        }
        Editor(Editor &) = delete;

        virtual bool isSingleton() { return true; }

        void edit(TileMap &t);

        private:
        TileMap *currentTileMap;
        uint32_t valToInsert;
        Editor() : currentTileMap(nullptr), valToInsert(0) { }
        static Editor *_inst;
    };
}