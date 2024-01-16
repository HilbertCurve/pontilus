#pragma once

#include <glm/glm.hpp>

#include "ecs/Component.h"

namespace Pontilus
{
    namespace ECS
    {
        class Transform : public Component
        {
            public:
            glm::vec3 pos;
            glm::vec3 whd;
            glm::vec3 rot;
            Transform() = default;
            Transform(glm::vec3 _pos, glm::vec3 _whd, glm::vec3 _rot) :
                    pos(_pos), whd(_whd), rot(_rot) {}
            virtual ~Transform() { }
            virtual int update(double);
            virtual bool operator==(Transform &);
        };
    }
}