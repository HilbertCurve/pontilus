#include "Texture.h"
#include "stb/stb_image.h"

namespace Pontilus
{
    namespace Renderer
    {
        namespace Texture
        {
            Texture &initTexture(const char *filepath)
            {
                Texture tex;
                tex.filepath = filepath;

                glGenTextures(1, &(tex.texID));
                glBindTexture(GL_TEXTURE_2D, tex.texID);

                // Set texture parameters
                // Repeat image in both directions
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // When stretching the image, pixelate
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                //When shrinking an image, pixelate
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                GLint *width;
                GLint *height;
                GLint *channels;
                stbi_set_flip_vertically_on_load(true);

                stbi_uc *image = stbi_load(filepath, width, height, channels, 0);

                if (image != nullptr)
                {
                    tex.width = *width;
                    tex.height = *height;

                    if (*channels == 3)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                    else if (*channels == 4)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                    else
                        fprintf(stderr, "Unknown number of channels: \"%d\"", *channels);
                }
                else
                {
                    fprintf(stderr, "Error: Could not load image \"%s\"", filepath);
                }

                stbi_image_free(image);
                return tex;
            }

            void bindTexture(Texture &t)
            {
                glBindTexture(GL_TEXTURE_2D, t.texID);
            }

            void unbindTexture(Texture &t)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }
}