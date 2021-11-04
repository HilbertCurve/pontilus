#include "Math.h"

namespace Pontilus
{
    namespace Math
    {
        int signOrZero(float a)
        {
            if (a > 0.0f) return 1;
            else if (a < 0.0f) return -1;
            else return 0;
        }
    }
}