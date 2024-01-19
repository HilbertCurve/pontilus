#include "renderer/Texture.h"

#include <math.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "renderer/Renderer.h"

namespace Pontilus
{
    namespace Renderer
    {
        // If width or height is zero, texture widths or heights are the full dimension of the texture
        IconMap::IconMap(const char *filepath, int textureWidth, int textureHeight, int padding) {
            this->filepath = filepath;

            glGenTextures(1, &(this->texID));
            glBindTexture(GL_TEXTURE_2D, this->texID);

            // Set texture parameters
            // Repeat image in both directions
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // When stretching the image, pixelate
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            // When shrinking an image, pixelate
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            GLint *width = new int;
            GLint *height = new int;
            GLint *channels = new int{0};
            stbi_set_flip_vertically_on_load(true);

            stbi_uc *image = stbi_load(filepath, width, height, channels, 4);

            if (image != nullptr)
            {
                this->width = *width;
                this->height = *height;

                if (*channels == 3)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                else if (*channels == 4)
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
                else
                {
                    fprintf(stderr, "Unknown number of channels: \"%d\"\n", *channels);
                    exit(-1);
                }
            }
            else
            {
                fprintf(stderr, "Error: Could not load image \"%s\"\n", filepath);
                exit(-1);
            }

            stbi_image_free(image);
            this->width = *width;
            this->height = *height;

            if (debugMode())
            {
                printf("Rendering Image: %s\nWidth: %d\nHeight: %d\nNumber of Channels: %d\n", filepath, *width, *height, *channels);
                printf("texID: %d\n\n", this->texID);
            }

            this->textureWidth = textureWidth == 0 ? this->width : textureWidth;
            this->textureHeight = textureHeight == 0 ? this->height : textureHeight;
            this->padding = padding;

            delete width;
            delete height;
            delete channels;

            RendererController::get().registerIconMap(*this);
        }

        Texture IconMap::getTexture(int index) {
            // NOTE: TEXCOORDS GO FROM 0.0 TO 1.0!!!
            Texture tex = {};
            tex.source = this;
            // get offset from top
            int pixelsFromTop = std::floor(index * this->textureWidth / this->width) * (this->textureHeight + this->padding);
            // get offset from left
            int pixelsFromLeft = (index * this->textureWidth + this->padding) % this->width;
            
            glm::vec2 pos1 = {pixelsFromLeft, pixelsFromTop};
            glm::vec2 pos2 = {pixelsFromLeft + this->textureWidth, pixelsFromTop + this->textureHeight};

            pos1.y = this->height - pos1.y;
            pos1 /= glm::vec2{this->width, this->height};

            pos2.y = this->height - pos2.y;
            pos2 /= glm::vec2{this->width, this->height};

            // insert texcoords
            float coords[] = 
            {
                pos2.x, pos1.y,
                pos1.x, pos1.y,
                pos1.x, pos2.y,
                pos2.x, pos2.y
            };

            for (int i = 0; i < 8; i++) 
            {
                tex.texCoords[i] = coords[i];
            }

            return tex;
        }

        Texture IconMap::emptyTexture() {
            return Texture{nullptr, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
        }
    }
}
