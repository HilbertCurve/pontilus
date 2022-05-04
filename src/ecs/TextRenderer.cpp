#include "ecs/TextRenderer.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            void TextRenderer::init(const char *text, Renderer::Font &f)
            {
                this->text = text;
                this->font = &f;
            }

            int TextRenderer::toRData(Renderer::rData &r, unsigned int rOffset)
            {
                using namespace Pontilus::Renderer;

                __pAssert(!((rOffset + this->text.size()) * 4 >= r.vertCount), "rData not big enough to hold UI data!");
                __pAssert(this->font != nullptr, "Text must be initialized with a font.");

                int stride = rOffset * getLayoutLen(r) * 4;

                // for each character in the text, add a default quad
                glm::vec3 posAccumulate = {0.0f, 0.0f, 0.0f};
                const float scale = this->font->fontSize / (this->font->ascent - this->font->descent);
                
                for (int i = 0; i < this->text.length(); i++)
                {
                    char c = this->text[i];
                    Glyph g = getGlyph(*this->font, c);

                    ////////          ////////
                    // Insertion into rData //
                    ////////          ////////

                    // kern depending on the previous char

                    // kern please
                    if (i != 0)
                    {
                        posAccumulate += screenToWorldSize({scale * stbtt_GetCodepointKernAdvance(&(g.parent->info), this->text[i - 1], this->text[i]), 0.0f});
                    }

                    for (int j = 0; j < 4; j++)
                    {
                        glm::vec3 orientation;

                        switch (j)
                        {
                            case 0: orientation = {1.0f * g.width, 1.0f * g.height, 0.0f}; break;
                            case 1: orientation = {0.0f * g.width, 1.0f * g.height, 0.0f}; break;
                            case 2: orientation = {0.0f * g.width, 0.0f * g.height, 0.0f}; break;
                            case 3: orientation = {1.0f * g.width, 0.0f * g.height, 0.0f}; break;
                        }

                        float xDiff = 0;
                        if (this->mode == TextMode::CENTER_LEFT)
                        {
                            xDiff = this->parent->width / 2.0f;
                        }

                        off_len result = getAttribMetaData(r, PONT_POS);
                        if (result.second >= 3 * sizeof(float))
                        {
                            // instead of position by bottom corner
                            this->parent->pos += orientation - glm::vec3{xDiff, g.height, 0.0f} + posAccumulate + glm::vec3{0.0f, g.descent, 0.0f};

                            // TODO: just use memcpy, bonehead.
                            for (int k = 0; k < 3; k++)
                            {
                                ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&this->parent->pos)[k];
                            }

                            this->parent->pos -= orientation - glm::vec3{xDiff, g.height, 0.0f} + posAccumulate + glm::vec3{0.0f, g.descent, 0.0f};
                        }
                        
                        result = getAttribMetaData(r, PONT_COLOR);
                        if (result.second >= 4 * sizeof(float))
                        {
                            for (int k = 0; k < 4; k++)
                            {
                                ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&this->color)[k];
                            }
                        }

                        result = getAttribMetaData(r, PONT_TEXCOORD);
                        if (result.second >= 2 * sizeof(float))
                        {
                            for (int k = 0; k < 2; k++)
                            {
                                ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&g.texCoords)[j * 2 + k];
                            }
                        }

                        result = getAttribMetaData(r, PONT_TEXID);
                        if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                        {
                            if (this->font == nullptr)
                            {
                                *(float *)((char *)r.data + result.first + stride) = 0.0f;
                            }
                            else
                            {
                                *(float *)((char *)r.data + result.first + stride) = this->font->texID;
                            }
                        }
                        stride += getLayoutLen(r);
                    }

                    ////////                                   ////////
                    // Spacing of chars based on Text specifications //
                    ////////                                   ////////

                    // if char is a space, check if total width of next word causes overflow.
                    int nextWordLength = 0;
                    if (c == ' ')
                    {
                        if (i + 1 != this->text.length())
                        {
                            int next = i + 1;
                            char curr = '\0';
                            while (curr != ' ' && next < this->text.length())
                            {
                                curr = this->text.at(next);
                                nextWordLength += getGlyph(*this->font, curr).width;
                                next++;
                            }
                        }
                    }

                    posAccumulate.x += g.width;

                    // if align is to the left
                    if (nextWordLength + posAccumulate.x > this->parent->width && i + 1 < this->text.length() && this->text[i + 1] != ' ')
                    {
                        posAccumulate.x = 0;
                        posAccumulate.y += screenToWorldSize(glm::vec2(0.0f, g.parent->ascent - g.parent->descent)).y;
                    }

                }
                
                __pAssert(scale == this->font->fontSize / (this->font->ascent - this->font->descent), "A font was trampled on!");

                return stride / (4 * getLayoutLen(r));
            }

            void TextRenderer::toRData(Renderer::rData &r, unsigned int rOffset, Renderer::vProp property)
            {
                // TODO
                return;
            }
        }
    }
}
