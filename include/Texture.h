/* date = October 11th 2021 2:35 pm */

#ifndef _PONTILUS_TEXTURE_H
#define _PONTILUS_TEXTURE_H

#include <GL/gl.h>

namespace Pontilus
{
    namespace Graphics
    {
        struct Texture
        {
            const char *filepath;
            GLuint texID;
            GLint width, height;
            bool beingUsed;
        };

        void initTexture(const char *filepath, Texture &tex);

        void bindTexture(Texture &t);
        void unbindTexture(Texture &t);

    }
}

#endif