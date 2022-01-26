/* date = January 14th 2022 10:21 am */

#include <vector>

#include "ecs/Component.h"
#include "ecs/SpriteRenderer.h"
#include "graphics/Texture.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            class Animation : public Component
            {
                public:
                Animation() = default;

                // all starts and ends are inclusive
                void init(Graphics::IconMap &im, int start, int end, bool loopAtEnds);

                std::vector<Graphics::Texture> textures;
                bool loopAtEnds;

                void addTexture(Graphics::Texture &t, int index);
                void pushTexture(Graphics::Texture &t);
                void pushTextures(Graphics::IconMap &im, int start, int end);
                void removeTexture(int index);
                void removeTextures(int start, int end);
                bool setAnimationBounds(int start, int end);
                bool next();
                bool previous();

                private:
                SpriteRenderer *spr;
                unsigned int index = 0;
                unsigned int currAnimationStart = 0, currAnimationEnd = 0;

                bool checkParentForSpr();
            };
        }
    }
}