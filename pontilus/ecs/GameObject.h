/* date = October 24th 2021 2:26 pm */

#pragma once

#include <glm/glm.hpp>

#include "ecs/Component.h"
#include "graphics/rData.h"
#include "graphics/Primitive.h"
#include "graphics/Texture.h"

namespace Pontilus
{
    namespace Engine 
    {
        namespace ECS
        {
            struct Component;

            class GameObject
            {
                public:
                glm::vec3 pos;
                glm::vec4 color;
                float width, height;
                std::vector<Component *> components = std::vector<Component *>();
                
                // should I overload contructor?? it's basically the same as {} contruction
                void init(glm::vec3 pos, glm::vec4 color, float width, float height);
                
                void addComponent(Component &c);
                Component *getComponent(const std::type_info &ti);
                void removeComponent(const std::type_info &ti);
            };
        }
    }
}
