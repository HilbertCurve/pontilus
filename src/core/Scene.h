/* date = October 30th 2021 3:16 pm */

#pragma once

#include <vector>

#include "ecs/EntityBuilder.h"
#include "ecs/Entity.h"
#include "renderer/Camera.h"
#include "renderer/rData.h"

namespace Pontilus
{
    typedef void (* _init)();
    typedef void (* _update)(double dt);
    typedef void (* _clean)();

    namespace ECS
    {
        class Entity;
    }

    class Scene
    {
        public:
        virtual ~Scene() = default;

        Renderer::Camera *getCamera() const;
        void setCamera(Renderer::Camera *c);

        Scene() = default;
        // SHOULD SCENE HAVE THESE????????
        // probably not, should be inheritance. I'm fighting demons
        virtual void init() = 0;

        virtual void update(double) = 0;

        virtual void clean() = 0;


        ECS::Entity *spawn();

        template<class T>
        ECS::Entity *build() {
            ECS::EntityBuilder *builder = new T();

            auto obj = this->spawn();
            builder->build(obj);

            delete builder;

            return obj;
        }

        ECS::Entity *get(size_t id) const;
        void despawn(size_t id);
        // DEPRECATED: use spawns
        // void addObject(ECS::GameObject obj);
        // void removeObject(int id);

        void updateObjects(double dt) const;
        void freeObjects();

        bool isUsed() { return this->used; }
        void setUsed(bool _used) { this->used = _used; }

        private:
        Renderer::Camera *camera{};
        std::vector<ECS::Entity *> objs = std::vector<ECS::Entity *>();
        bool used = false;
    };
}

