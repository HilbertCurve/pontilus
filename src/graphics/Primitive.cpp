#include "graphics/Primitive.h"

#include "utils/Utils.h"

namespace Pontilus
{
    namespace Renderer
    {
        namespace Primitives
        {
            Primitive QUAD = Primitive{4, 6, GL_TRIANGLES, [](int *elements, int location)
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

            Primitive LINE = Primitive{2, 2, GL_LINE, [](int *elements, int location)
                {
                    int index = location * 2;
                    int offset = location * 2;

                    elements[index + 0] = 0 + offset;
                    elements[index + 1] = 1 + offset;
                }};
            Primitive TRIANGLE = Primitive{3, 3, GL_TRIANGLES, [](int *elements, int location)
                {
                    int index = location * 3;
                    int offset = location * 3;
                    
                    elements[index + 0] = 0 + offset;
                    elements[index + 1] = 1 + offset;
                    elements[index + 2] = 2 + offset;
                }};
            Primitive POINT = Primitive{1, 1, GL_POINT, [](int *elements, int location)
                {
                    int index = location;
                    int offset = location;

                    elements[index] = offset;
                }};
            Primitive MESH = Primitive{0, 0, GL_TRIANGLES, [](int *elements, int location)
                {
                    // do nothing; indices are specified on a case-by-case basis for meshes
                }};
            Primitive NONE = Primitive{0, 0, 0, [](int *elements, int location)
                {
                    __pWarning("rData with NONE primitive attempted to be buffered.");
                }};
        }
    }
}
