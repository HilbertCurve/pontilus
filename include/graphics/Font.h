/* date = December 10th 2021 7:05 pm */

#pragma once

#include <GL/gl.h>

#include <stb/stb_truetype.h>

namespace Pontilus
{
    namespace Graphics
    {
        struct Font;

        struct Glyph
        {
            const Font *parent;
            float texCoords[8];
            float width, height, descent; // descent would be present in the char 'j'
        };

        struct Font
        {
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
            stbtt_fontinfo info;
            Glyph glyphs[98];

            GLuint texID;
            bool beingUsed;
        };

        void initFont(Font &f, const char *filepath, unsigned int fontSize);
        void bindFont(Font &f);
        void unbindFont(Font &f);
        Glyph getGlyph(const Font &f, const char c);
    }
}