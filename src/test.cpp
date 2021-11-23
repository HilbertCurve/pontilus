#include <stdio.h>
#include <stdlib.h>

#include "physics2d/Body2D.h"
#include "physics2d/CollisionDetection.h"
#include "physics2d/CollisionDetection.h"

#define EXIT asm ("hlt")

int main()
{
    using namespace Pontilus::Physics2D;
    //__pError("oh no");
    glm::vec2 p = {10, 2};
    Triangle t = Triangle({-1, -2}, {0, 3}, {2, 4});

    __pAssert(!detectPointTriangle(p, t), "This assertion should not run");
    __pMessage("Assertion #1 has passed.");

    return 0;
}