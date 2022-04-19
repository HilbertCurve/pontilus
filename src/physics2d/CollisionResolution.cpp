#include "physics2d/CollisionResolution.h"
#include "physics2d/CollisionDetection.h"
#include "ecs/Body2D.h"
#include "utils/PMath.h"

#include <glm/glm.hpp>

namespace Pontilus
{
    namespace Physics2D
    {
        static void freeResolveAABBOptimized(pData data)
        {
            if (!data.colliding) return;
            if (data.colliders.first->mass == IMMOVABLE && data.colliders.second->mass == IMMOVABLE) return;

            Engine::ECS::AABB first = dynamic_cast<Engine::ECS::AABB &>(*data.colliders.first);
            Engine::ECS::AABB second = dynamic_cast<Engine::ECS::AABB &>(*data.colliders.second);

            using namespace glm;
            vec2 avgCollisionPoint = glm::vec2();

            for (vec2 v : data.collisionPoints)
            {
                avgCollisionPoint += v;
            }

            avgCollisionPoint /= data.collisionPoints.size();

            // find collision normal
            vec2 vNorm = first.velocity - second.velocity;
            Engine::ECS::AABB tFirst = {first.min + vNorm * 1.0f/60.0f, first.max + vNorm * 1.0f/60.0f};
            if (!detectAABBAABB(tFirst, second).colliding) {
                printf("i\n");
                return;
            }

            vec2 vNorm1;
            float slope = (first.center.y - first.lastCenter.y) / (first.center.x - first.lastCenter.x);
            bool bx = Math::between(second.min.x, first.center.x, second.max.x);
            bool by = Math::between(second.min.y, first.center.y, second.max.y);
            if (bx && !by)
            {
                vNorm1 = {vNorm.x, -vNorm.y};
            }
            else if (by && !bx)
            {
                vNorm1 = {-vNorm.x, vNorm.y};
            }
            else
            {
                if (abs(slope) > 1.0f)
                {
                    vNorm1 = {-vNorm.x, vNorm.y};
                }
                else
                {
                    vNorm1 = {vNorm.x, -vNorm.y};
                }
            }
            
            vec2 vNorm2 = vNorm;
            
            // conservation of momentum

            float massRatio = first.mass / (first.mass + second.mass);

            vNorm1 *= (massRatio);
            vNorm2 *= (1- massRatio);

            data.colliders.first->velocity = vNorm1;
            data.colliders.second->velocity = vNorm2;

            // sanity
            data.colliders.first->angularVelocity = 0;
            data.colliders.second->angularVelocity = 0;
        }

        void freeResolve(pData data)
        {
            if (typeid(*data.colliders.first) == typeid(Engine::ECS::AABB) && typeid(*data.colliders.second) == typeid(Engine::ECS::AABB))
            {
                freeResolveAABBOptimized(data);
                return; 
            }
            
            // broken, all of it
            static int i = 0;
            using namespace glm;

            if (!data.colliding) return;
            if (data.colliders.first->mass == IMMOVABLE && data.colliders.first->mass == IMMOVABLE) return;

            vec2 avgCollisionPoint = glm::vec2();

            for (vec2 v : data.collisionPoints)
            {
                avgCollisionPoint += v;
            }

            avgCollisionPoint /= data.collisionPoints.size();

            // determine collision normal based on body velocities
            vec2 vNorm = data.colliders.first->velocity - data.colliders.second->velocity;

            // determine angular-velocity-based normals
            float sDistToA = Math::distSquared(avgCollisionPoint, data.colliders.first->center);
            float sDistToB = Math::distSquared(avgCollisionPoint, data.colliders.second->center);

            vec2 dVa = avgCollisionPoint - data.colliders.first->center;
            // this is sketch; i worked out the formula for it like 3 months ago and can't guarantee this works (if it is it's a steal compared to sin() and cos() stuff)
            float iNormA_y = sDistToA * data.colliders.first->angularVelocity * dVa.x; 
            float iNormA_x = sDistToA * data.colliders.first->angularVelocity * -dVa.y;

            vec2 dVb = avgCollisionPoint - data.colliders.second->center;
            float iNormB_y = sDistToB * data.colliders.second->angularVelocity * dVb.x;
            float iNormB_x = sDistToB * data.colliders.second->angularVelocity * -dVb.y;

            vec2 iNormA = { iNormA_x, iNormA_y };
            vec2 iNormB = { iNormB_x, iNormB_y };

            vec2 cNorm = vNorm + iNormA + iNormB;

            // determine tangential force based on vector between center
            // and collision point of each body; this is where we split
            // some of the momentum into body b
            vec2 cNormA = cNorm * (data.colliders.first->mass / (data.colliders.first->mass + data.colliders.second->mass));
            vec2 cNormB = -(cNorm - cNormA);

            vec2 aNormal = normalize(dVa);
            vec2 bNormal = normalize(dVb);

            vec2 pNormA = aNormal;
            vec2 pNormB = bNormal;

            // tangent stuff is for angular momentum
            vec2 tNormA = (vec2) { -aNormal.y, aNormal.x };
            vec2 tNormB = (vec2) { -bNormal.y, bNormal.x };

            vec2 aImpulse = pNormA * dot(cNormA, pNormA);
            vec2 bImpulse = pNormB * dot(cNormB, pNormB);

            tNormA *= dot(cNormA, tNormA);
            tNormB *= dot(cNormB, tNormB);

            float aMoment = ((cNormA.x + cNormA.y) / sDistToA) * (1/cNormA.x + 1/cNormA.y);
            float bMoment = ((cNormB.x + cNormB.y) / sDistToB) * (1/cNormB.x + 1/cNormB.y);

            // we'll see if that violated the law of conservation of momentum :pain:

            data.colliders.first->velocity += aImpulse;
            data.colliders.second->velocity += bImpulse;
            data.colliders.first->angularVelocity += aMoment;
            data.colliders.second->angularVelocity += bMoment;
        }

        void constraintResolve(Constraint &cst)
        {
            // go cry in the corner
            return;
        }
    }
}