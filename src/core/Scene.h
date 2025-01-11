/* date = October 30th 2021 3:16 pm */

#pragma once

#include <vector>

#include "ecs/EntityBuilder.h"
#include "ecs/Entity.h"
#include "renderer/Camera.h"
#include "renderer/rData.h"

namespace Pontilus
{
    /*
    typedef void (* _init)();
    typedef void (* _update)(double dt);
    typedef void (* _clean)();
    */

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

        virtual void init() = 0;
        virtual void update(double) = 0;
        virtual void clean() = 0;

        ECS::Entity *spawn();
        ECS::Entity *spawn(const std::string &name);

        template<class T>
        ECS::Entity *build() {
            std::string name = std::string("entity_") + std::to_string(ECS::Entity::_id) + std::string("_auto");
            return this->build<T>(name);
        }
        template<class T>
        ECS::Entity *build(const std::string &name) {
            ECS::EntityBuilder *builder = new T();

            const auto obj = this->spawn(name);
            builder->build(obj);

            delete builder;

            return obj;
        }

        ECS::Entity *get(size_t id) const;
        ECS::Entity *get(const std::string &name) const;

        void despawn(size_t id);
        void despawn(const std::string &name);

        void updateObjects(double dt) const;
        void freeObjects();

        bool isUsed() const { return this->used; }
        void setUsed(const bool _used) { this->used = _used; }

    private:
        Renderer::Camera *camera{};
        std::vector<ECS::Entity *> objs = std::vector<ECS::Entity *>();
        bool used = false;
    };
}

