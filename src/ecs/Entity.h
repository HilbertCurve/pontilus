/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "ecs/Component.h"
#include "renderer/rData.h"

namespace Pontilus
{
    class Scene;
    namespace ECS
    {
        class Component;

        class Entity
        {
        public:
            ~Entity();
            static int _id;
            std::vector<Component *> components = std::vector<Component *>();
            const int id = ++_id;
            Scene *currentScene;
            std::string name;
            static const std::string::const_pointer SET_NAME_PREFIX;
            static const std::string::const_pointer AUTO_NAME_PREFIX;

            Entity *addComponent(Component *c);

            template<typename T>
            T *getComponent() {
                for (Component *component : this->components) {
                    if (T *c = dynamic_cast<T*>(component)) {
                        return c;
                    }
                }

                return nullptr;
            }

            template<typename T>
            Entity *build() {
                static T *builder = new T();
                builder->build(this);
                return this;
            }

            Component *getComponent(const std::type_info &ti);
            // returns component for you to free it at end of lifecycle
            Component *removeComponent(const std::type_info &ti);

            void update(double dt);
        };
    }
}

