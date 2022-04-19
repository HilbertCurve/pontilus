/* date = November 11th 2021 3:09 pm */

#pragma once

#include <vector>

#include "ecs/Body2D.h"
#include "utils/Utils.h"

namespace Pontilus
{
    namespace Physics2D
    {
        typedef Pair<Engine::ECS::Body2D *, Engine::ECS::Body2D *> BodyPair;
        struct pData
        {
            bool colliding;
            BodyPair colliders;
            std::vector<glm::vec2> collisionPoints;
        };

        pData detectCollision(Engine::ECS::Body2D &body1, Engine::ECS::Body2D &body2);
        bool isInside(glm::vec2 p, Engine::ECS::Body2D &body);

        bool detectPointCircle(glm::vec2 p, Engine::ECS::Circle &c);
        bool detectPointAABB(glm::vec2 p, Engine::ECS::AABB &a);
        bool detectPointBox(glm::vec2 p, Engine::ECS::Box2D &b);
        bool detectPointTriangle(glm::vec2 p, Engine::ECS::Triangle &t);

        pData detectCircleCircle(Engine::ECS::Circle &c1, Engine::ECS::Circle &c2);
        pData detectCircleAABB(Engine::ECS::Circle &c, Engine::ECS::AABB &a);
        pData detectCircleBox(Engine::ECS::Circle &c, Engine::ECS::Box2D &b);
        pData detectAABBAABB(Engine::ECS::AABB &a1, Engine::ECS::AABB &a2);
        pData detectAABBBox(Engine::ECS::AABB &a, Engine::ECS::Box2D &b);
        pData detectBoxBox(Engine::ECS::Box2D &b1, Engine::ECS::Box2D &b2);
    }
}