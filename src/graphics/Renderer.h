/* date = August 26th 2021 8:40 pm */

#ifndef _PONTILUS_RENDERER_H
#define _PONTILUS_RENDERER_H

#include "graphics/rData.h"
#include "graphics/Texture.h"
#include "graphics/Font.h"

namespace Pontilus
{
    namespace Renderer
    {
        extern Renderer::rData quadPool;
        extern Renderer::rData modelPool;
        extern Renderer::rData linePool;
        extern Renderer::rData fullWindowQuad;
        extern Renderer::rData pointLightPool;
        extern Renderer::IconMap *iconPool[8];
        extern Renderer::Font *fontPool[8];
        extern int iconPoolStackPointer;
        extern int fontPoolStackPointer;

        void start();
        void render();
        void modelRender();
        void postRender();
        void debugRender();

        //void renderRData(Renderer::rData &r, Renderer::Primitive mode, unsigned int numObjects);
        void close();
    }
}

#endif //_PONTILUS_RENDERER_H
