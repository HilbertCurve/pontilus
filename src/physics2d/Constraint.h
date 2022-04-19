/* date = November 29th 2021 10:41 am */

#pragma once

#include "physics2d/CollisionDetection.h"

namespace Pontilus
{
    namespace Physics2D
    {
        class Constraint
        {
            public:
            BodyPair bodies;
            int priority;
            virtual void resolve() = 0;
        };

        // two points based on an object's local coordinates must stay equal to
        // two points based on another object's local coordinates
        class VertexConstraint : public Constraint
        {
            glm::vec2 localPoints1;
            glm::vec2 localPoints2;
            void resolve();
        };

        // one point based on an object's local coordinates must stay equal to
        // one point based on another object's local coordinates
        class JointConstraint : public Constraint
        {
            glm::vec2 localPoint1;
            glm::vec2 localPoint2;
            void resolve();
        };

        // one point must stay on the line between two other points on another body
        class SlideConstraint : public Constraint
        {
            glm::vec2 localPoint1;
            glm::vec2 localPoints2;
            void resolve();
        };
    }
}