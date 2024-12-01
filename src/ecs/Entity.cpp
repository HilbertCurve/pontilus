#include "core/Application.h"
#include "core/Scene.h"
#include "ecs/Entity.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace ECS
    {
        int Entity::_id = 0;

        Entity::~Entity() {
            for (Component *c : components) {
                if (!c->isSingleton())
                    c->clear();

                delete c;
            }

            _pMessage("GameObject of id %u deleted.", this->id);
        }

        void Entity::addComponent(Component *c)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component *ca = this->components.at(i);
                if (typeid(*ca) == typeid(*c))
                {
                    _pWarning("Component of type %s already in GameObject %p.", typeid(*c).name(), this);
                    return;
                }
            }

            this->components.push_back(c);
            c->parent = this;
            
            //if (debugMode())
                _pMessage("Component added to GameObject of id %i of type %s.", this->id, typeid(*c).name());
        }

        Component *Entity::getComponent(const std::type_info &ti)
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

        Component *Entity::removeComponent(const std::type_info &ti)
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
                        _pMessage("Component removed from gameObject %p of type %s.", this, ti.name());
                    return ca;
                }
                i++;
            }

            _pWarning("Component of type %s not found in GameObject %p.", ti.name(), this);
            return nullptr;
        }

        void Entity::update(double dt)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component *c = this->components.at(i);
                if (int err = c->update(dt) != 0)
                {
                    _pError("Update failed: error code %u\n", err);
                }
            }
        }
    }
}
