#pragma once

#include <vector>
#include <GL/gl.h>

#include "Primitive.h"
#include "Rend.h"

namespace Pontilus
{
    namespace Graphics
    {
        struct RenderBatch
        {
            Primitive type;

            std::vector<Rend> elements;
        };


    }
}