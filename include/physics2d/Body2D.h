/* date = November 11th 2021 2:49 pm */

#pragma once

#include "glm/glm.hpp"

namespace Pontilus
{
    namespace Physics2D
    {
        class Body2D
        {
            public:
            float mass;
            glm::vec2 velocity;
            float angularVelocity;
            float friction;
        };
        
        class AABB : public Body2D
        {
            public:
            glm::vec2 min;
            glm::vec2 max;
        };

        class Circle : public Body2D
        {
            public:
            glm::vec2 center;
            float radius;
        };

        class Box2D : public Body2D
        {
            public:
            glm::vec2 min;
            glm::vec2 max;
            float rotation;
        };

        template<unsigned int n>
        class Polygon : public Body2D
        {
            public:
            static int numVerts = n;
            glm::vec2 vertices[n];
        };
    }
}