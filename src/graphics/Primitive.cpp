#include "Primitive.h"

namespace Pontilus
{
    namespace Graphics
    {
        namespace Primitives
        {
            Primitive QUAD = Primitive{4, 6, GL_TRIANGLES, [](int *elements, int location)
                                       {
                                           int index = location * 6;

                                           elements[index + 0] = 3;
                                           elements[index + 1] = 2;
                                           elements[index + 2] = 0;

                                           elements[index + 3] = 0;
                                           elements[index + 4] = 2;
                                           elements[index + 5] = 1;
                                       }};

            Primitive LINE = Primitive{2, 2, GL_LINE, [](int *elements, int location)
                                       {
                                           int index = location * 2;

                                           elements[index + 0] = 0;
                                           elements[index + 1] = 1;
                                       }};
        }
    }
}