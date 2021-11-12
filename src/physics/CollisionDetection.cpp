#include "physics2d/CollisionDetection.h"
#include "PMath.h"

namespace Pontilus
{
    namespace Physics2D
    {
        using namespace Pontilus::Math;
        pData detectCircleCircle(Circle &c1, Circle &c2)
        {
            // if the distance between the centers is greater than 
            // the radius of the circles, return false.

            pData ret;
            ret.colliders = BodyPair{c1, c2};

            if (distSquared(c1.center, c2.center) > (c1.radius + c2.radius))
            {
                ret.colliding = false;
                return ret;
            }
        }
    }
}