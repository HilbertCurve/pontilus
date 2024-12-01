#include "renderer/TextRenderer.h"
#include "ecs/Transform.h"

namespace Pontilus
{
    namespace Renderer
    {
        void TextRenderer::init(const char *text, Renderer::Font &f, glm::vec4 color)
        {
            this->text = text;
            this->font = &f;
            this->color = color;
        }

        int TextRenderer::toRData(Renderer::rData &r)
        {
            using namespace Pontilus::ECS;


            _pAssert(this->font != nullptr, "Text must be initialized with a font.");

            int stride = r.dataOffset;

            Transform *transformPointer = (Transform *)this->parent->getComponent(typeid(Transform));
            if (!transformPointer) {
                _pWarning("Attempt to render text without a transform; I don't know where to draw!");
                return 1;
            }
            Transform _t = *transformPointer;

            // for each character in the text, add a default quad
            glm::vec3 posAccumulate = {0.0f, 0.0f, 0.0f};
            const float scale = this->font->fontSize / (this->font->ascent - this->font->descent);
            // this is for making text fit inside textboxes
            //const float heightAdjust = -screenToWorldSize({0.0f, (float) this->font->ascent}).y;
            const float heightAdjust = 0.0;
            const float lineHeight = screenToWorldSize(glm::vec2(0.0f, this->font->ascent - this->font->descent)).y;
            for (unsigned int i = 0; i < this->text.length(); i++)
            {
                char c = this->text[i];

                if (c == '\n')
                {
                    posAccumulate.y += lineHeight;
                    posAccumulate.x = 0.0f;
                    continue;
                }

                Glyph g = this->font->get(c);

                ////////          ////////
                // Insertion into rData //
                ////////          ////////

                // kern depending on the previous char

                // kern please
                if (i != 0)
                {
                    //posAccumulate += screenToWorldSize({scale * stbtt_GetCodepointKernAdvance(&(g.parent->info), this->text[i - 1], this->text[i]), 0.0f});
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
                    float yDiff = 0;
                    if (this->mode == TextMode::CENTER_LEFT)
                    {
                        xDiff = _t.whd.x / 2.0f;
                    }
                    yDiff = _t.whd.y / 2.0f;

                    rData::off_len result = r.getAttribMetaData(PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        // instead position by bottom corner
                        // also move everything down a bit
                        _t.pos += orientation - glm::vec3{xDiff, g.height - yDiff - heightAdjust, 0.0f} + posAccumulate + glm::vec3{0.0f, g.descent, 0.0f};

                        // TODO: just use memcpy, bonehead.
                        for (int k = 0; k < 3; k++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&_t.pos)[k];
                        }

                        _t.pos -= orientation - glm::vec3{xDiff, g.height - yDiff - heightAdjust, 0.0f} + posAccumulate + glm::vec3{0.0f, g.descent, 0.0f};
                    }
                    
                    result = r.getAttribMetaData(PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int k = 0; k < 4; k++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&this->color)[k];
                        }
                    }

                    result = r.getAttribMetaData(PONT_TEXCOORD);
                    if (result.second >= 2 * sizeof(float))
                    {
                        for (int k = 0; k < 2; k++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[k] = ((float *)&g.texCoords)[j * 2 + k];
                        }
                    }

                    result = r.getAttribMetaData(PONT_TEXID);
                    if (result.second == 1 * sizeof(float)) // I'd be very confused if there was more than one texID.
                    {
                        if (this->font == nullptr)
                        {
                            *(float *)((char *)r.data + result.first + stride) = 0.0f;
                        }
                        else
                        {
                            *(float *)((char *)r.data + result.first + stride) = this->font->id();
                        }
                    }
                    stride += r.getLayoutLen();
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
                            nextWordLength += this->font->get(curr).width;
                            next++;
                        }
                    }
                }

                posAccumulate.x += g.width;

                // if align is to the left
                if (nextWordLength + posAccumulate.x > _t.whd.x && i + 1 < this->text.length() && this->text[i + 1] != ' ')
                {
                    posAccumulate.x = 0;
                    posAccumulate.y += lineHeight;
                }

            }
            
            _pAssert(scale == this->font->fontSize / (this->font->ascent - this->font->descent), "A font was trampled on!");

            r.dataOffset = stride;

            return 0;
        }
    }
}
