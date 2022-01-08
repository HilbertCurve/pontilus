#include "core/Application.h"
#include "ecs/GameObject.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            void GameObject::init(glm::vec3 pos, glm::vec4 color, float width, float height)
            {
                this->pos = pos;
                this->color = color;
                this->width = width;
                this->height = height;
            }

            void GameObject::addComponent(Component &c)
            {
                for (int i = 0; i < this->components.size(); i++)
                {
                    Component *ca = this->components.at(i);
                    if (typeid(*ca) == typeid(c))
                    {
                        __pWarning("Component of type %s already in GameObject %p.", typeid(c).name(), this);
                        return;
                    }
                }

                this->components.push_back(&c);
                c.parent = this;
                
                if (debugMode())
                    __pMessage("Component added to gameObject %p of type %s.", this, typeid(c).name());
            }

            Component *GameObject::getComponent(const std::type_info &ti)
            {
                for (int i = 0; i < this->components.size(); i++)
                {
                    Component *ca = this->components.at(i);
                    if (typeid(*ca) == ti)
                    {
                        return ca;
                    }
                }

                return nullptr;
            }

            void GameObject::removeComponent(const std::type_info &ti)
            {
                int i = 0;
                for (int i = 0; i < this->components.size(); i++)
                {
                    Component *ca = this->components.at(i);
                    if (typeid(*ca) == ti)
                    {
                        this->components.erase(this->components.begin() + i);
                        ca->parent = nullptr;
                        if (debugMode())
                            __pMessage("Component removed from gameObject %p of type %s.", this, ti.name());
                        return;
                    }
                    i++;
                }

                __pWarning("Component of type %s not found in GameObject %p.", ti.name(), this);
            }
        }
    }
}