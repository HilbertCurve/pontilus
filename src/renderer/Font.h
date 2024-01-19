/* date = December 10th 2021 7:05 pm */

#pragma once

#include <glad/gl.h>

#include <vendor/stb/stb_truetype.h>

namespace Pontilus
{
    namespace Renderer
    {
        struct Font;

        struct Glyph
        {
            const Font *parent;
            float texCoords[8];
            float width, height, descent; // descent would be present in the char 'j'
        };

        class Font
        {
            public:
            Font(const char *filepath, unsigned int fontsize);
            Glyph get(const char c);

            GLuint id() { return texID; }

            // it seems like a lot of fields in this struct are in the glyph struct...

            const char *filepath;
            union // functionally they're the same
            {
                int width;
                int fontSize;
            };
            // is this bloat? i know i can just use stbtt_GetFontVMetrics, but this could be faster...
            int ascent;
            int descent;
            int lineGap;

            // this is for finding kern info, advance, and other font
            // metrics. 
            private:
            stbtt_fontinfo info;
            Glyph glyphs[98];

            GLuint texID;
        };
    }
}
