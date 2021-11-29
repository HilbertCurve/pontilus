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
            ret.colliders = BodyPair{&c1, &c2};

            float sumRadius = c1.radius + c2.radius;

            ret.colliding = true;

            if (distSquared(c1.center, c2.center) > sumRadius * sumRadius)
            {
                ret.colliding = false;
            }

            // for the line connecting the two circles' centers, find the point
            // halfway between where the line intersects the two circles. set that
            // to the collision point.
            // TODO: set the collision points to wherever the two circles intersect.
            if (ret.colliding)
            {
                glm::vec2 collisionPoint;

                glm::vec2 pC1 = lerp(c1.center, c2.center, c1.radius / dist(c1.center, c2.center));
                glm::vec2 pC2 = lerp(c2.center, c1.center, c2.radius / dist(c1.center, c2.center));

                collisionPoint = (pC1 + pC2) / 2.0f;

                ret.collisionPoints.push_back(collisionPoint);
            }
            
            return ret;
        }

        pData detectCircleAABB(Circle &c, AABB &a)
        {
            // find the closest point on the perimeter of the AABB
            // to the center of the circle, and if that point is
            // inside the circle, return true

            pData ret;
            ret.colliders = BodyPair{&c, &a};

            // if center is inside AABB, return true
            if (detectPointAABB(c.center, a)) 
            {
                ret.colliding = true;
                return ret;
            }

            glm::vec2 closestPoint = c.center;

            closestPoint.x = closest(a.min.x, a.max.x, c.center.x);
            closestPoint.y = closest(a.min.y, a.max.y, c.center.y);

            ret.colliding = detectPointCircle(closestPoint, c);

            // set collision point to the point halfway between 
            // the end of the circle and the AABB
            // TODO: find points where the AABB and circle intersect
            // and add those as collision points
            if (ret.colliding)
            {
                glm::vec2 collisionPoint = c.center;
                float d;

                if (closestPoint.x != a.min.x && closestPoint.x != a.max.x)
                {
                    d = closestPoint.y - c.center.y;
                    float lerpFactor = ((c.radius + d) / 2.0f) / c.radius;

                    collisionPoint.y = lerp(c.center.y, c.center.y + c.radius, lerpFactor);

                }
                else if (closestPoint.y != a.min.y && closestPoint.y != a.max.y)
                {
                    d = closestPoint.x - c.center.x;
                    float lerpFactor = ((c.radius + d) / 2.0f) / c.radius;

                    collisionPoint.x = lerp(c.center.x, c.center.x + c.radius, lerpFactor);
                }
                else
                {
                    d = dist(closestPoint, c.center);

                    // we divide by d here because we're lerping to the
                    // closest point rather than the point on the circle
                    float lerpFactor = ((c.radius + d) / 2.0f) / d;

                    collisionPoint = lerp(c.center, closestPoint, lerpFactor);
                }

                ret.collisionPoints.push_back(collisionPoint);
            }

            return ret;
        }

        pData detectAABBAABB(AABB &a1, AABB &a2)
        {
            // if the center of a1 is inside the AABB aTranspose,
            // where aTranspose's minimum is a1's minimum minus a2's
            // width and height and aTranspose's maximum is a1's 
            // maximum plus a2's width and height, return true

            pData ret;
            ret.colliders = BodyPair{&a1, &a2};
            
            // a2's width and height
            glm::vec2 a2wh = a2.max - a2.min;

            AABB aTranspose = AABB(a1.min - a2wh, a1.max + a2wh);

            ret.colliding = detectPointAABB((a1.max + a1.min) / 2.0f, aTranspose);

            // find corners inside a2. adjacent sides must be intersecting.
            if (ret.colliding)
            {
                // corners of aabb:
                /*
                 *   1   0
                 *
                 *   2   3
                 */

                glm::vec2 a1verts[] = 
                {
                    a1.max,
                    {a1.max.y, a1.min.x},
                    a1.min,
                    {a1.max.x, a1.min.y}
                };

                std::vector<glm::vec2> insideVerts;

                for (glm::vec2 v : a1verts)
                {
                    if (detectPointAABB(v, a2))
                    {
                        insideVerts.push_back(v);
                    }
                }
                
                switch (insideVerts.size())
                {
                    case 0:
                    {
                        // if there are no inside vertices, one rectangle is on top of another.
                        // return the center of the rectangle made by their intersection.
                        if (insideVerts.size() == 0)
                        {
                            if (a2.max.x - a2.min.x > a1.max.x - a1.min.x)
                            {
                                ret.collisionPoints.push_back({(a1.max.x + a1.min.x) / 2, (a2.max.y + a2.min.y) / 2});
                            }
                            else
                            {
                                ret.collisionPoints.push_back({(a2.max.x + a2.min.x) / 2, (a1.max.y + a1.min.y) / 2});
                            }
                        }
                    } break;
                    case 1:
                    {
                        // one corner of the aabb has intersected another. determine
                        // which corner and find collision points accordingly.
                        glm::vec2 v = insideVerts[0];
                        glm::vec2 collisionPoints[2]; // higher one is index 0

                        if (between(v.x, a2.max.x, a1.max.x))
                        {
                            collisionPoints[0].x = v.x;
                            collisionPoints[1].x = a2.max.x;
                        }
                        else
                        {
                            collisionPoints[0].x = a2.min.x;
                            collisionPoints[1].x = a1.max.x;
                        }
                        if (between(v.y, a2.max.y, a1.max.y))
                        {
                            collisionPoints[0].y = v.y;
                            collisionPoints[1].y = a2.max.y;
                        }
                        else
                        {
                            collisionPoints[0].y = a2.min.y;
                            collisionPoints[1].y = a1.max.y;
                        }

                        ret.collisionPoints.push_back(collisionPoints[0]);
                        ret.collisionPoints.push_back(collisionPoints[1]);
                    } break;
                    case 2:
                    {
                        // one side is inside another aabb. determine which one and find
                        // collision points accordingly.
                        glm::vec2 collisionPoints[2];
                        
                        if (between(a2.min.x, insideVerts[0].x, a2.max.x) && between(a2.min.x, insideVerts[1].x, a2.max.x))
                        {
                            if (insideVerts[0].y == a1.min.y && insideVerts[1].y == a1.min.y)
                            {
                                collisionPoints[0] = glm::vec2(a1.min.x, a2.max.y);
                                collisionPoints[0] = glm::vec2(a1.max.x, a2.max.y);
                            }
                            else if (insideVerts[0].y == a1.max.y && insideVerts[1].y == a1.max.y)
                            {
                                collisionPoints[0] = glm::vec2(a1.min.x, a2.min.y);
                                collisionPoints[0] = glm::vec2(a1.max.x, a2.min.y);
                            }
                            else
                            {
                                // we should never reach this place; something stinky has happened.
                                __pError("Impossible condition met.");
                            }
                        }
                        else if (between(a2.min.y, insideVerts[0].y, a2.max.y) && between(a2.min.y, insideVerts[1].y, a2.max.y))
                        {
                            if (insideVerts[0].x == a1.min.x && insideVerts[1].x == a1.min.x)
                            {
                                collisionPoints[0] = glm::vec2(a2.max.x, a1.min.y);
                                collisionPoints[0] = glm::vec2(a2.max.x, a1.max.y);
                            }
                            else if (insideVerts[0].x == a1.max.x && insideVerts[1].x == a1.max.x)
                            {
                                collisionPoints[0] = glm::vec2(a2.min.x, a1.min.y);
                                collisionPoints[0] = glm::vec2(a2.min.x, a1.max.y);
                            }
                            else
                            {
                                // we should never reach this place; something stinky has happened.
                                __pError("Impossible condition met.");
                            }
                        }
                        else
                        {
                            // yet another stinky-impossible condition.
                            __pError("Impossible condition met.");
                        }
                    } break;
                    case 3:
                    {
                        // this is mathematically impossible; something stinky has happened.
                        __pError("Two AABBs cannot intersect each other in 3 places.");
                    } break;
                    case 4:
                    {
                        // one whole aabb is inside another. return the center of the smaller aabb.
                        glm::vec2 center;
                        center.x = (a1.max.x + a1.min.x) / 2;
                        center.y = (a1.max.y + a1.min.y) / 2;

                        ret.collisionPoints.push_back(center);
                    }
                }
            }
            
            return ret;
        }

        pData detectAABBBox(AABB &a, Box2D &b)
        {
            // tbd
            return {};
        }
    }
}
