#include "math/Math.h"

namespace Pontilus
{
    namespace Math
    {
        int sign(float a)
        {
            if (a >= 0.0f) return 1;
            else return -1;
        }
        int signOrZero(float a)
        {
            if (a > 0.0f) return 1;
            else if (a < 0.0f) return -1;
            else return 0;
        }
    }
}