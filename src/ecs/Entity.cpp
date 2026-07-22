#include <pontilus/core/Application.h>
#include <pontilus/core/Scene.h>
#include <pontilus/ecs/Entity.h>
#include <pontilus/utils/Utils.h>

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

            auto a_prefix = AUTO_NAME_PREFIX;
            auto s_prefix = SET_NAME_PREFIX;
            if (this->name.substr(this->name.length() - std::strlen(a_prefix)) == a_prefix) {
                _pMessage("Entity of id %u deleted.", this->id);
            } else {
                auto demangle = this->name.substr(0, this->name.length() - std::strlen(s_prefix));
                _pMessage("Entity of name \"%s\" deleted.", demangle.c_str());
            }
        }

        Entity *Entity::addComponent(Component *c)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                if (typeid(*this->components.at(i)) == typeid(*c))
                {
                    auto a_prefix = AUTO_NAME_PREFIX;
                    auto s_prefix = SET_NAME_PREFIX;
                    if (this->name.substr(this->name.length() - std::strlen(a_prefix)) == a_prefix) {
                        _pWarning("Component of type %s already in Entity %p.", typeid(*c).name(), this);
                    } else {
                        auto demangle = this->name.substr(0, this->name.length() - std::strlen(s_prefix));
                        _pWarning("Component of type %s already in Entity \"%s\".", typeid(*c).name(), demangle.c_str());
                    }
                    return this;
                }
            }

            this->components.push_back(c);
            c->parent = this;

            //if (debugMode())
            auto a_prefix = AUTO_NAME_PREFIX;
            auto s_prefix = SET_NAME_PREFIX;
            if (this->name.substr(this->name.length() - std::strlen(a_prefix)) == a_prefix) {
                _pMessage("Component added to Entity of id %i of type %s.", this->id, typeid(*c).name());
            } else {
                auto demangle = this->name.substr(0, this->name.length() - std::strlen(s_prefix));
                _pMessage("Component added to Entity \"%s\" of type %s.", demangle.c_str(), typeid(*c).name());
            }
            return this;
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
                    auto a_prefix = AUTO_NAME_PREFIX;
                    auto s_prefix = SET_NAME_PREFIX;
                    if (this->name.substr(this->name.length() - std::strlen(a_prefix)) == a_prefix) {
                        _pMessage("Component removed from Entity %p of type %s.", this, ti.name());
                    } else {
                        auto demangle = this->name.substr(0, this->name.length() - std::strlen(s_prefix));
                        _pMessage("Component removed from Entity \"%s\" of type %s.", demangle.c_str(), ti.name());
                    }
                    return ca;
                }
                i++;
            }

            _pWarning("Component of type %s not found in Entity %p.", ti.name(), this);
            return nullptr;
        }

        void Entity::update(double dt)
        {
            for (uint32_t i = 0; i < this->components.size(); i++)
            {
                Component *c = this->components.at(i);
                if (int err = c->update(dt) != 0)
                {
                    auto a_prefix = AUTO_NAME_PREFIX;
                    auto s_prefix = SET_NAME_PREFIX;
                    if (this->name.substr(this->name.length() - std::strlen(a_prefix)) == a_prefix) {
                        _pError("Update failed on Entity of id %d: error code %u\n", this->id, err);
                    } else {
                        auto demangle = this->name.substr(0, this->name.length() - std::strlen(s_prefix));
                        _pError("Update failed on Entity of name \"%s\": error code %u\n", demangle.c_str(), err);
                    }

                }
            }
        }
        const std::string::const_pointer Entity::SET_NAME_PREFIX = "_explicit";
        const std::string::const_pointer Entity::AUTO_NAME_PREFIX = "_auto";
    }
}
