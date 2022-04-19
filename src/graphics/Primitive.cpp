#include "graphics/Primitive.h"

namespace Pontilus
{
    namespace Graphics
    {
        namespace Primitives
        {
            Primitive QUAD = Primitive{4, 6, GL_TRIANGLES, Renderer::PONT_GAME, [](int *elements, int location)
                                       {
                                           int index = location * 6;
                                           int offset = location * 4;

                                           elements[index + 0] = 3 + offset;
                                           elements[index + 1] = 2 + offset;
                                           elements[index + 2] = 0 + offset;

                                           elements[index + 3] = 0 + offset;
                                           elements[index + 4] = 2 + offset;
                                           elements[index + 5] = 1 + offset;
                                       }};

            Primitive LINE = Primitive{2, 2, GL_LINE, Renderer::PONT_DEBUG, [](int *elements, int location)
                                       {
                                           int index = location * 2;
                                           int offset = location * 2;

                                           elements[index + 0] = 0 + offset;
                                           elements[index + 1] = 1 + offset;
                                       }};
            Primitive POINT = Primitive{1, 1, GL_POINT, Renderer::PONT_DEBUG, [](int *elements, int location)
                                        {
                                            int index = location;
                                            int offset = location;

                                            elements[index] = offset;
                                        }};
        }
    }
}