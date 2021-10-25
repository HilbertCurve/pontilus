/* date = October 21th 2021 8:31 am */

#pragma once

#include <GL/gl.h>

#include "Renderer.h"
#include "Rend.h"

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
            elementsCallback generateIndices;
        };

        namespace Primitives
        {
            extern Primitive QUAD;

            extern Primitive LINE;
        }
    }
}
