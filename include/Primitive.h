/* date = October 21th 2021 8:31 am */

#pragma once

#include <GL/gl.h>

#include "Renderer.h"
#include "rData.h"

namespace Pontilus
{
    namespace Graphics
    {
        typedef void (*elementsCallback)(int *elements, int location);
        struct Primitive
        {
            unsigned int vertexCount;
            unsigned int elementSize;
            GLenum renderType;
            Renderer::rMode renderMode;
            elementsCallback generateIndices;
        };

        namespace Primitives
        {
            extern Primitive QUAD;
            extern Primitive LINE;
            extern Primitive TRIANGLE;
            extern Primitive RECTANGLE;
            extern Primitive CIRCLE;
            extern Primitive POINT;
        }
    }
}
