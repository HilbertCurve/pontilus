/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "core/Scene.h"
#include "ecs/Component.h"
#include "renderer/rData.h"
#include "renderer/Primitive.h"
#include "renderer/Texture.h"

namespace Pontilus
{
    struct Scene;
    namespace ECS
    {
        struct Component;

        class GameObject
        {
            public:
            ~GameObject();
            static int _id;
            std::vector<Component *> components = std::vector<Component *>();
            const int id = ++_id;
            Scene *currentScene;
            
            void addComponent(Component *c);
            Component *getComponent(const std::type_info &ti);
            // returns component for you to free it at end of lifecycle
            Component *removeComponent(const std::type_info &ti);

            void update(double dt);
        };
    }
}

