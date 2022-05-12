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
                static int _id;

                glm::vec3 pos;
                // x axis, y axis, z axis rotation
                glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
                float width, height;
                std::vector<Component *> components = std::vector<Component *>();
                int id = ++_id;
                Engine::Scene *currentScene;

                void init(glm::vec3 pos, float width, float height);
                
                void addComponent(Component &c);
                Component *getComponent(const std::type_info &ti);
                void removeComponent(const std::type_info &ti);
            };
        }
    }
}

