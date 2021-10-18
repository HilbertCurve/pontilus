/* date = October 11th 2021 2:19 pm */

#ifndef _PONTILUS_OBJ_2D_H
#define _PONTILUS_OBJ_2D_H

#include <vector>
#include <GL/gl.h>

#include "Texture.h"

namespace Pontilus
{
    namespace Model
    {
        struct Obj2D
        {
            GLfloat *vbo;
            GLint size;

            Renderer::Texture::Texture t;
        };
    }
}

#endif //_PONTILUS_OBJ_2D_H
