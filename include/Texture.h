/* date = October 11th 2021 2:35 pm */

#ifndef _PONTILUS_TEXTURE_H
#define _PONTILUS_TEXTURE_H

#include <GLES3/gl3.h>

namespace Pontilus
{
    namespace Renderer
    {
        namespace Texture
        {
            struct Texture
            {
                const char *filepath;
                GLuint texID;
                GLint width, height;
                bool beingUsed;
            };

            Texture &initTexture(const char *filepath);

            void bindTexture(Texture &t);
            void unbindTexture(Texture &t);
        }
    }
}

#endif