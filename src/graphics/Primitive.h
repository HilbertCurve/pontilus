/* date = October 21th 2021 8:31 am */

#pragma once

#include <glad/gl.h>

namespace Pontilus
{
    namespace Renderer
    {
        typedef void (*elementsCallback)(int *elements, int location);
        struct Primitive
        {
            unsigned int vertexCount;
            unsigned int elementSize;
            GLenum renderType;
            elementsCallback generateIndices;
        };

        namespace Primitives
        {
            extern Primitive QUAD;
            extern Primitive LINE;
            extern Primitive TRIANGLE;
            extern Primitive CIRCLE;
            extern Primitive POINT;
            extern Primitive MESH;
            extern Primitive NONE;
        }
    }
}

