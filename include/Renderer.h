/* date = August 26th 2021 8:40 pm */

#ifndef _PONTILUS_RENDERER_H
#define _PONTILUS_RENDERER_H

#include "Camera.h"

namespace Pontilus
{
    namespace Renderer
    {
        typedef struct RenderableData
        {
            GLfloat *vbo;
            GLint size;
        };
        void start();
        void render();
    }
}

#endif //_PONTILUS_RENDERER_H
