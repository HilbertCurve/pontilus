/* date = January 14th 2022 10:21 am */
#pragma once

#include <vector>

#include "ecs/Component.h"
#include "renderer/SpriteRenderer.h"
#include "renderer/Texture.h"

namespace Pontilus
{
    namespace Library
    {
        class Animation : public ECS::Component
        {
            public:
            Animation(Renderer::IconMap &im, int start, int end, bool loopAtEnds)
            {
                this->init(im, start, end, loopAtEnds);
            }

            // all starts and ends are inclusive
            void init(Renderer::IconMap &im, int start, int end, bool loopAtEnds);

            std::vector<Renderer::Texture> textures;
            bool loopAtEnds;

            void addTexture(Renderer::Texture &t, int index);
            void pushTexture(Renderer::Texture &t);
            void pushTextures(Renderer::IconMap &im, int start, int end);
            void removeTexture(int index);
            void removeTextures(int start, int end);
            bool setAnimationBounds(int start, int end);
            bool next();
            bool previous();

            virtual int update(double dt) { return 0; }

            virtual ~Animation() = default;

            private:
            Renderer::SpriteRenderer *spr;
            int index = 0;
            unsigned int currAnimationStart = 0, currAnimationEnd = 0;

            bool checkParentForSpr();
        };
    }
}
