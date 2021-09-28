/* date = September 28th 2021 9:27 am */

#ifndef _PONTILUS_OBJ_3D_H
#define _PONTILUS_OBJ_3D_H

#include <GLES3/gl3.h>

namespace Pontilus
{
    namespace Model
    {
        struct Obj3D
        {
            GLfloat *vertexBufferData;
        };

        void loadObjFromBinary(const char *filepath, Obj3D &dest);
    }
}

#endif