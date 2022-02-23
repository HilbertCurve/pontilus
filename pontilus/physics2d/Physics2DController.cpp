#include "physics2d/Physics2DController.h"
#include "physics2d/CollisionDetection.h"
#include "physics2d/CollisionResolution.h"

namespace Pontilus
{
    namespace Physics2D
    {
        static std::vector<Engine::ECS::Body2D *> bodies;
        void update(double dt)
        {
            for (int i = 0; i < bodies.size(); i++) 
            {
                Engine::ECS::Body2D * b = bodies.at(i);
                b->update(dt);
                b->parent->pos = glm::vec3(b->center.x, b->center.y, b->parent->pos.z);
            }

            // naive way
            for (int i = 0; i < bodies.size(); i++)
            {
                for (int j = i + 1; j < bodies.size(); j++)
                {
                    auto b1 = bodies.at(i);
                    auto b2 = bodies.at(j);
                    auto result = detectCollision(*b1, *b2);
                    if (result.colliding)
                        freeResolve(result);
                }
            }
        }

        void fixedUpdate()
        {
            update(1.0f/60.0f);
        }

        void addBody(Engine::ECS::Body2D *b)
        {
            bodies.push_back(b);
        }
    }
}