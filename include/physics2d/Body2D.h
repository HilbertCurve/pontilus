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
            Body2D() = default;
            float mass;
            glm::vec2 velocity;
            float angularVelocity;
            float friction;

            // renderer information
            glm::vec4 color;
            virtual float lineCount();
        };
        
        class AABB : public Body2D
        {
            public:
            AABB(glm::vec2 min, glm::vec2 max);
            glm::vec2 min;
            glm::vec2 max;

            float lineCount();
        };

        class Circle : public Body2D
        {
            public:
            Circle(glm::vec2 center, float radius);
            glm::vec2 center;
            float radius;

            float lineCount();
        };

        class Box2D : public Body2D
        {
            public:
            Box2D(glm::vec2 center, float width, float height, float rotation);
            glm::vec2 center;
            float width, height;
            float rotation;

            float lineCount();
        };

        template<unsigned int N>
        class Polygon : public Body2D
        {
            public:
            Polygon() = default;
            Polygon(unsigned int numVerts);
            static const int numVerts = N;
            glm::vec2 vertices[N];

            float lineCount();
        };

        class Triangle : public Polygon<3>
        {
            public:
            Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
        };
    }
}