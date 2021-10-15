#include "Texture.h"
#include "stb/stb_image.h"

namespace Pontilus
{
    namespace Renderer
    {
        namespace Texture
        {
            void initTexture(const char *filepath, Texture &tex)
            {
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

                GLint *width = new int;
                GLint *height = new int;
                GLint *channels = new int{0};
                stbi_set_flip_vertically_on_load(true);

                stbi_uc *image = stbi_load(filepath, width, height, channels, 4);

                if (image != nullptr)
                {
                    tex.width = *width;
                    tex.height = *height;

                    if (*channels == 3)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                    else if (*channels == 4)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                    else 
                    {
                        fprintf(stderr, "Unknown number of channels: \"%d\"\n", *channels); exit(-1);
                    }
                }
                else
                {
                    fprintf(stderr, "Error: Could not load image \"%s\"\n", filepath); exit(-1);
                }

                stbi_image_free(image);
                tex.width = *width;
                tex.height = *height;

                printf("%d, %d, %d\n", *width, *height, *channels);

                delete width;
                delete height;
                delete channels;
            }

            void bindTexture(Texture &t)
            {
                glBindTexture(GL_TEXTURE_2D, t.texID);
                t.beingUsed = true;
            }

            void unbindTexture(Texture &t)
            {
                glBindTexture(GL_TEXTURE_2D, 0);
                t.beingUsed = false;
            }
        }
    }
}