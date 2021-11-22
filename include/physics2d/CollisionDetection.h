/* date = November 11th 2021 3:09 pm */

#pragma once

#include "physics2d/Body2D.h"
#include "Utils.h"

namespace Pontilus
{
    namespace Physics2D
    {
        typedef Pair<Body2D, Body2D> BodyPair;
        struct pData
        {
            bool colliding;
            BodyPair colliders;
            glm::vec2 collisionPoint;
        };

        pData detectCollision(Body2D &body1, Body2D &body2);
        bool isInside(glm::vec2 p, Body2D &body);

        bool detectPointCircle(glm::vec2 p, Circle &c);
        bool detectPointAABB(glm::vec2 p, AABB &a);
        bool detectPointBox(glm::vec2 p, Box2D &b);

        pData detectCircleCircle(Circle &c1, Circle &c2);
        pData detectCircleAABB(Circle &c, AABB &a);
        pData detectCircleBox(Circle &c, Box2D &b);
        pData detectAABBAABB(AABB &a1, AABB &a2);
        pData detectAABBBox(AABB &a, Box2D &b);
        pData detectBoxBox(Box2D &b1, Box2D &b2);
    }
}