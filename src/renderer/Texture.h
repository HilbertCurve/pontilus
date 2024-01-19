/* date = October 11th 2021 2:35 pm */

#pragma once

#include <glad/gl.h>

namespace Pontilus
{
    namespace Renderer
    {
        class IconMap;

        struct Texture
        {
            IconMap *source;
            float texCoords[8];
        };

        class IconMap
        {
            public:
            IconMap(const char *filepath, int textureWidth, int textureHeight, int padding);

            Texture getTexture(int index);
            static Texture emptyTexture();

            GLuint id() { return texID; }

            private:

            const char *filepath;
            GLuint texID;
            int width, height;
            int textureWidth, textureHeight;
            int padding;
        };
    }
}
