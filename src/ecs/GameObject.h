/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "core/Scene.h"
#include "ecs/Component.h"
#include "graphics/rData.h"
#include "graphics/Primitive.h"
#include "graphics/Texture.h"

namespace Pontilus
{
    namespace Engine 
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
                Engine::Scene *currentScene;
                
                void addComponent(Component *c);
                Component *getComponent(const std::type_info &ti);
                // returns component for you to free it at end of lifecycle
                Component *removeComponent(const std::type_info &ti);

                void update(double dt);
            };
        }
    }
}

