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
                GameObject() : pos{glm::vec3(0.0f, 0.0f, 0.0f)}, width{0.0f}, height{0.0f} {}
                GameObject(glm::vec3 _pos, float _width, float _height) :
                    pos{_pos}, width{_width}, height{_height} {}
                
                ~GameObject();

                static int _id;

                glm::vec3 pos;
                glm::vec2 velocity;
                // x axis, y axis, z axis rotation
                glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
                float width, height;
                std::vector<Component *> components = std::vector<Component *>();
                int id = ++_id;
                Engine::Scene *currentScene;

                void init(glm::vec3 pos, float width, float height);
                
                void addComponent(Component *c);
                Component *getComponent(const std::type_info &ti);
                // returns component for you to free it at end of lifecycle
                Component *removeComponent(const std::type_info &ti);

                void update(double dt);
            };
        }
    }
}

