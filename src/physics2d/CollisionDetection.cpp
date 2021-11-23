#include "physics2d/CollisionDetection.h"
#include "PMath.h"

namespace Pontilus
{
    namespace Physics2D
    {
        using namespace Pontilus::Math;

        bool detectPointCircle(glm::vec2 p, Circle &c)
        {
            // check if distance between the point and the center 
            // of the circle is less than the circle's radius

            if (distSquared(p, c.center) > c.radius * c.radius)
            {
                return false;
            }

            return true;
        }

        bool detectPointAABB(glm::vec2 p, AABB &a)
        {
            // check if the point has x and y values between the
            // left-right and top-bottom sides respectively of the AABB

            bool insideX;
            bool insideY;

            if (p.x > a.min.x && p.x < a.max.x)
            {
                insideX = true;
            }
            if (p.y > a.min.y && p.y < a.max.y)
            {
                insideY = true;
            }

            return insideX && insideY;
        }

        bool detectPointTriangle(glm::vec2 p, Triangle &t)
        {
            // for all three lines of the triangle, find out
            // if we should check for the point above the line
            // or below it. XNOR the result with whether or not
            // the point is above the line for each side of the 
            // triangle, and return all the XNORs &&'d together.
            
            Line line0 = Line(t.vertices[1], t.vertices[2]);
            Line line1 = Line(t.vertices[2], t.vertices[0]);
            Line line2 = Line(t.vertices[0], t.vertices[1]);

            bool line0Up = pointAboveLine(t.vertices[0], line0);
            bool line1Up = pointAboveLine(t.vertices[1], line1);
            bool line2Up = pointAboveLine(t.vertices[2], line2);

            if (line0Up == pointAboveLine(p, line0) && 
                line1Up == pointAboveLine(p, line1) &&
                line2Up == pointAboveLine(p, line2))
                return true;
            
            return false;
        }

        pData detectCircleCircle(Circle &c1, Circle &c2)
        {
            // if the distance between the centers is greater than 
            // the radius of the circles, return false.

            pData ret;
            ret.colliders = BodyPair{c1, c2};

            float sumRadius = c1.radius + c2.radius;

            if (distSquared(c1.center, c2.center) > sumRadius * sumRadius)
            {
                ret.colliding = false;
            }
            
            return ret;
        }

        pData detectCircleAABB(Circle &c, AABB &a)
        {
            // 

            return {};
        }

        pData detectAABBAABB(AABB &a1, AABB &a2)
        {
            // if the center of a1 is inside the AABB aTranspose,
            // where aTranspose's minimum is a1's minimum minus a2's
            // width and height and aTranspose's maximum is a1's 
            // maximum plus a2's width and height, return true

            pData ret;
            ret.colliders = BodyPair{a1, a2};
            
            // a2's width and height
            glm::vec2 a2wh = a2.max - a2.min;

            AABB aTranspose = AABB(a1.min - a2wh, a1.max + a2wh);

            ret.colliding = detectPointAABB((a1.max + a1.min) / 2.0f, aTranspose);
            
            return ret;
        }

        pData detectAABBBox(AABB &a, Box2D &b)
        {
            // tbd
            return {};
        }
    }
}