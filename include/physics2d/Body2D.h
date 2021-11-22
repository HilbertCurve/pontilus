/* date = November 11th 2021 2:49 pm */

#pragma once

#include <glm/glm.hpp>

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
            AABB(glm::vec2 min, glm::vec2 max);
            glm::vec2 min;
            glm::vec2 max;
        };

        class Circle : public Body2D
        {
            public:
            Circle(glm::vec2 center, float radius);
            glm::vec2 center;
            float radius;
        };

        class Box2D : public Body2D
        {
            public:
            Box2D(glm::vec2 center, float width, float height, float rotation);
            glm::vec2 center;
            float width, height;
            float rotation;
        };

        class Polygon : public Body2D
        {
            public:
            Polygon(unsigned int numVerts);
            const int numVerts;
            glm::vec2 vertices[];
        };
    }
}