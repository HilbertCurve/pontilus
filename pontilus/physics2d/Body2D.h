/* date = November 11th 2021 2:49 pm */

#pragma once

#include <glm/glm.hpp>
#include "ecs/Component.h"
#include "graphics/Renderer.h"

#define IMMOVABLE 0.0f

namespace Pontilus
{
    namespace Physics2D
    {
        class Body2D : public Engine::ECS::Component//, public Graphics::Renderable
        {
            public:
            Body2D() = default;
            virtual void update(double dt) = 0;
            void init();
            float mass;
            glm::vec2 velocity = {0.0f, 0.0f};
            float angularVelocity = 0.0f;
            float friction = 0.0f;
            glm::vec2 center;

            // renderer information
            glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
            //virtual int toRData(Graphics::rData &r, unsigned int rOffset) = 0;
        };
        
        class AABB : public Body2D
        {
            public:
            AABB() = default;
            AABB(glm::vec2 min, glm::vec2 max);
            void update(double dt);
            glm::vec2 min;
            glm::vec2 max;

            //int toRData(Graphics::rData &r, unsigned int rOffset);
        };

        class Circle : public Body2D
        {
            public:
            Circle(glm::vec2 center, float radius);
            void update(double dt);
            float radius;

            //int toRData(Graphics::rData &r, unsigned int rOffset);
        };

        class Box2D : public Body2D
        {
            public:
            Box2D(glm::vec2 center, float width, float height, float rotation);
            void update(double dt);
            float width, height;
            float rotation;

            //int toRData(Graphics::rData &r, unsigned int rOffset);
        };

        template<unsigned int N>
        class Polygon : public Body2D
        {
            public:
            Polygon() = default;
            Polygon(unsigned int numVerts);
            void update(double dt);
            static const int numVerts = N;
            glm::vec2 vertices[N];

            //int toRData(Graphics::rData &r, unsigned int rOffset);
        };

        class Triangle : public Polygon<3>
        {
            public:
            Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
        };
    }
}
