/* date = October 21th 2021 8:31 am */

#ifndef _PONTILUS_PRIMITIVE_H
#define _PONTILUS_PRIMITIVE_H

#include <GL/gl.h>

namespace Pontilus
{
    namespace Model
    {
        typedef void (*primCallback)(int *elements, int location);
        struct Primitive
        {
            unsigned int vertexCount;
            GLenum renderType;
            primCallback generateIndices;
        };

        namespace Primitives
        {
            Primitive QUAD = Primitive{4, GL_TRIANGLE, [](int *elements, int location)
                                       {
                                           int index = location * 6;

                                           elements[location + 0] = 3;
                                           elements[location + 1] = 2;
                                           elements[location + 2] = 0;

                                           elements[location + 3] = 0;
                                           elements[location + 4] = 2;
                                           elements[location + 5] = 1;
                                       }};
            Primitive LINE = Primitive{2, GL_LINE, [](int *elements, int location)
                                       {
                                           int index = location * 2;
                                           
                                           elements[location + 0] = 0;
                                           elements[location + 1] = 1;
                                       }};
        }
    }
}

#endif // _PONTILUS_PRIMITIVE_H