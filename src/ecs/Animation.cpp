#include "ecs/Animation.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            void Animation::init(Renderer::IconMap &im, int start, int end, bool loopAtEnds) 
            {
                this->loopAtEnds = loopAtEnds;

                for (int i = start; i <= end; i++)
                {
                    this->textures.push_back(Renderer::getTexture(im, i));
                }
            }

            void Animation::addTexture(Renderer::Texture &t, int index)
            {
                this->textures.insert(this->textures.begin() + index, t);
            }

            void Animation::pushTexture(Renderer::Texture &t)
            {
                this->textures.push_back(t);
            }

            void Animation::pushTextures(Renderer::IconMap &im, int start, int end)
            {
                for (int i = start; i <= end; i++)
                {
                    this->textures.push_back(Renderer::getTexture(im, i));
                }
            }

            void Animation::removeTexture(int index)
            {
                this->textures.erase(this->textures.begin() + index, this->textures.begin() + index + 1);
            }

            void Animation::removeTextures(int start, int end)
            {
                this->textures.erase(this->textures.begin() + start, this->textures.begin() + end + 1);
            }

            bool Animation::checkParentForSpr()
            {
                if (this->spr == nullptr || !(this->spr->parent == this->parent))
                {
                    if (!(this->spr = dynamic_cast<SpriteRenderer *> (
                        this->parent->getComponent(typeid(SpriteRenderer)))))
                    {
                        __pWarning("GameObject at %p has Animation component but not SpriteRenderer component.", this->parent);
                        return false;
                    }
                }

                return true;
            }

            bool Animation::setAnimationBounds(int start, int end)
            {
                if (start < 0)
                {
                    __pWarning("Animation at %p can't have negative starting bound %d (for now).", this, start);
                }
                else if (end >= this->textures.size())
                {
                    __pWarning("Animation at %p can't have ending bound %d greater than highest index %ld (for now).", this, end, this->textures.size());
                }
                else
                {
                    this->currAnimationStart = start;
                    this->currAnimationEnd = end;
                    return true;
                }

                return false;
            }

            bool Animation::next()
            {
                if (this->checkParentForSpr() && !(!this->loopAtEnds && this->index >= this->currAnimationEnd))
                {
                    if (++this->index >= this->textures.size())
                    {
                        this->index = 0;
                    }
                    this->spr->tex = this->textures.at(this->index);
                    return true;
                }
                
                return false;
            }
            
            bool Animation::previous()
            {
                if (this->checkParentForSpr() && !(!this->loopAtEnds && this->index <= this->currAnimationStart))
                {
                    if (--this->index < 0)
                    {
                        this->index = this->textures.size() - 1;
                    }
                    this->spr->tex = this->textures.at(this->index);
                    return true;
                }
                
                return false;
            }
        }
    }
}
