/* date = October 11th 2021 2:35 pm */

#pragma once

#include <GL/gl.h>

namespace Pontilus
{
    namespace Renderer
    {
        struct IconMap
        {
            const char *filepath;
            GLuint texID;
            int width, height;
            int textureWidth, textureHeight;
            int padding;
            bool beingUsed;
        };

        struct Texture
        {
            IconMap *source;
            float texCoords[8];
        };

        void initIconMap(const char *filepath, IconMap &tex, int textureWidth, int textureHeight, int padding);

        void bindIconMap(IconMap &t);
        void unbindIconMap(IconMap &t);

        Texture getTexture(IconMap &im, int index);
    }
}
