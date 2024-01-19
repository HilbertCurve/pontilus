#include "renderer/Font.h"

#include <string>
#include <glm/glm.hpp>

#include <stb/stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#include "renderer/Renderer.h"
#include "utils/Utils.h"

#define _PONTILUS_FONT_TEX_WIDTH 512
#define _PONTILUS_FONT_TEX_HEIGHT 512

namespace Pontilus
{
    namespace Renderer
    {
        Font::Font(const char *fontname, unsigned int fontSize)
        {
            /* Load font (. ttf) file */

            File fontFile;
            loadFile(fontname, fontFile, true);

            stbtt_fontinfo info;
            bool loaded = stbtt_InitFont(&info, (unsigned char *) fontFile.buffer, 0) != 0;
            if (!loaded)
            {
                __pError("Font loading failed for font %s.", fontname);
            }

            this->info = info;

            const int bitmap_w = _PONTILUS_FONT_TEX_WIDTH;
            const int bitmap_h = _PONTILUS_FONT_TEX_HEIGHT;
            unsigned char *bitmap = (unsigned char *)malloc(bitmap_w * bitmap_h);
            // clean bitmap
            for (int i = 0; i < 512 * 512; i++)
            {
                bitmap[i] = 0;
            }

            this->fontSize = fontSize;
            float pixels = fontSize;
            float scale = stbtt_ScaleForPixelHeight(&info, pixels); /* scale = pixels / (ascent - descent) */

            /*
             * Get the measurement in the vertical direction
             * ascent: The height of the font from the baseline to the top;
             * descent: The height from baseline to bottom is usually negative;
             * lineGap: The distance between two fonts;
             * The line spacing is: ascent - descent + lineGap.
             */
            stbtt_GetFontVMetrics(&info, &this->ascent, &this->descent, &this->lineGap);

            /* Adjust word height according to zoom */
            this->ascent = roundf(this->ascent * scale);
            this->descent = roundf(this->descent * scale);

            /* Generate font texture */
            int x = 0, y = 0, advanceWidth = 0, leftSideBearing = 0, h;

            const int padding = 2; // two pixels of extra spacing between chars, so anti-aliasing doesn't mess something up.

            for (int i = 32; i < 127; ++i)
            {
                ////////                   ////////
                // Insert character into texture //
                ////////                   //////// 

                /*
                 * Get the measurement in the horizontal direction
                 * advanceWidth: currnet codepoint position;
                 * leftSideBearing: Left side position;
                 */
                stbtt_GetCodepointHMetrics(&info, (char)i, &advanceWidth, &leftSideBearing);

                // ensure we don't go out of bounds
                if (x + advanceWidth * scale + padding > bitmap_w)
                {
                    y += pixels + padding;
                    x = 0;
                }

                /* Gets the border of a character */
                int c_x1, c_y1, c_x2, c_y2;
                stbtt_GetCodepointBitmapBox(&info, (char)i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

                /* Calculate the y of the bitmap (different characters have different heights) */
                h = this->ascent + c_y1 + y;

                /* Render character */
                int byteOffset = x + roundf(leftSideBearing * scale) + (h * bitmap_w);
                stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_w, scale, scale, (char) i);

                ////////                  ////////
                // Generate corresponding glyph //
                ////////                  ////////

                float texcoords[8] = 
                {
                    (float) x + (advanceWidth + leftSideBearing) * scale, (float) h - c_y1 + c_y2,
                    (float) x + leftSideBearing * scale,                  (float) h - c_y1 + c_y2,
                    (float) x + leftSideBearing * scale,                  (float) h,
                    (float) x + (advanceWidth + leftSideBearing) * scale, (float) h,
                };

                glm::vec2 widthAndHeight = screenToWorldSize(glm::vec2{
                    texcoords[0] - texcoords[2], 
                    texcoords[3] - texcoords[5]});

                for (int j = 0; j < 8; j++)
                {
                    if (!(j % 2)) // is an x coordinate
                    {
                        texcoords[j] = texcoords[j] / bitmap_w; // percentage it is left
                    }
                    else
                    {
                        texcoords[j] = (bitmap_h - texcoords[j]) / bitmap_h; // percentage it is UP, not down
                    }
                }

                // at this point it's ready as an opengl texture
                memcpy((void *) this->glyphs[i - 32].texCoords, texcoords, 8 * sizeof(float));

                this->glyphs[i - 32].width = widthAndHeight.x;
                this->glyphs[i - 32].height = widthAndHeight.y;
                this->glyphs[i - 32].descent = screenToWorldSize(glm::vec2{0.0f, c_y2}).y;

                this->glyphs[i - 32].parent = this;

                /* Adjust x */
                x += roundf(advanceWidth * scale) + padding;
            }
            
            /*
            stbtt_BakeFontBitmap((unsigned char *) fontFile.buffer, 0, 64.0, bitmap,
                                 512, 512, 32, 96,
                                 this->characters);
                                */

            // at this point we can use bitmap as a gl texture
            unsigned int id = this->texID;
            glGenTextures(1, &(this->texID));
            glBindTexture(GL_TEXTURE_2D, this->texID);
            
            // flip bitmap, because opengl
            unsigned char *newBitmap = (unsigned char *) malloc(bitmap_w * bitmap_h);
            for (int i = 0; i < bitmap_w; i++)
            {
                for (int j = 0; j < bitmap_h; j++)
                {
                    newBitmap[i + j * bitmap_w] = bitmap[i + (bitmap_h - j) * bitmap_w];
                }
            }

            // GL_RED here refers to the fact that there's only one channel (the first one)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, newBitmap);

            // a swizzle maps some channels of a texture to different ones (in this case, we turn our
            // texture from a red-intensity to an rgba-intensity.) this lets the shader/vertex data
            // choose the color of the text.
            GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_RED};
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            free(bitmap);
            free(newBitmap);

            RendererController::get().registerFont(*this);
        }

        Glyph Font::get(const char c)
        {
            /*
            Glyph ret;

            ret.parent = &f;

            stbtt_aligned_quad q;
            // i don't really NEED these too much; all positioning is handled by the camera
            float dummyx = 0, dummyy = 0;
            stbtt_GetBakedQuad(this->characters, 512, 512, c - 32, &dummyx, &dummyy, &q, true);

            float texcoords[] =
                {
                    q.s1, q.t1,
                    q.s0, q.t1,
                    q.s0, q.t0,
                    q.s1, q.t0
                };
            
            memcpy(&ret.texCoords, texcoords, 8 * sizeof(float));

            glm::vec2 widthAndHeight = c != ' ' ? 
                screenToWorldSize(glm::vec2{q.x1 - q.x0, q.y1 - q.y0}) : 
                screenToWorldSize(glm::vec2{this->fontSize, 0.0f});
            ret.width = widthAndHeight.x;
            ret.height = widthAndHeight.y;

            static glm::vec2 sWidthAndHeight = screenToWorldSize(glm::vec2{this->fontSize / 2, 0.0f});

            // TODO: I don't know how to get the space character info, so I'm using '!' as a replacement.
            static Glyph space = { &f, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, this->glyphs[0].width, this->glyphs[0].height };
            */
            
            if (c < 32 || c > 127)
            {
                return this->glyphs[0];
            }

            return this->glyphs[c - 32];
        }
    }
}

