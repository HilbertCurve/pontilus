#include "ecs/GameObject.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            void GameObject::addComponent(Component &c)
            {
                for (auto &c : this->components)
                {
                    if (typeid(c) == typeid(*this))
                    {
                        __pWarning("Component of type %s already in GameObject %p.", typeid(c).name(), this);
                        return;
                    }
                }

                this->components.push_back(c);
            }

            Component &GameObject::getComponent(const std::type_info &ti) __THROW
            {
                for (auto &c : this->components)
                {
                    if (typeid(c) == ti)
                    {
                        return c;
                    }
                }

                __pWarning("Component of type %s not found in GameObject %p.", typeid(c).name(), this);
                throw std::exception();
            }

            void GameObject::removeComponent(const std::type_info &ti)
            {
                int i = 0;
                for (auto &c : this->components)
                {
                    if (typeid(c) == ti)
                    {
                        this->components.erase(this->components.begin() + i);
                        return;
                    }
                    i++;
                }

                __pWarning("Component of type %s not found in GameObject %p.", typeid(c).name(), this);
            }
        }
    }
}