#include "core/Application.h"
#include "core/Scene.h"
#include "ecs/GameObject.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace ECS
    {
        int GameObject::_id = 0;

        GameObject::~GameObject() {
            for (Component *c : components) {
                if (!c->isSingleton())
                    c->clear();

                delete c;
            }

            __pMessage("GameObject of id %u deleted.", this->id);
        }

        void GameObject::addComponent(Component *c)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component *ca = this->components.at(i);
                if (typeid(*ca) == typeid(c))
                {
                    __pWarning("Component of type %s already in GameObject %p.", typeid(c).name(), this);
                    return;
                }
            }

            this->components.push_back(c);
            c->parent = this;
            
            //if (debugMode())
                __pMessage("Component added to gameObject %p of type %s.", this, typeid(c).name());
        }

        Component *GameObject::getComponent(const std::type_info &ti)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component &ca = *this->components.at(i);
                if (typeid(ca) == ti)
                {
                    return &ca;
                }
            }

            return nullptr;
        }

        Component *GameObject::removeComponent(const std::type_info &ti)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component *ca = this->components.at(i);
                if (typeid(*ca) == ti)
                {
                    // TODO: proper clearing!
                    this->components.erase(this->components.begin() + i);
                    ca->parent = nullptr;
                    //if (debugMode())
                        __pMessage("Component removed from gameObject %p of type %s.", this, ti.name());
                    return ca;
                }
                i++;
            }

            __pWarning("Component of type %s not found in GameObject %p.", ti.name(), this);
            return nullptr;
        }

        void GameObject::update(double dt)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component *c = this->components.at(i);
                if (int err = c->update(dt) != 0)
                {
                    __pError("Update failed: error code %u\n", err);
                }
            }
        }
    }
}
