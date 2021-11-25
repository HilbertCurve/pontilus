/* date = August 26th 2021 8:40 pm */

#ifndef _PONTILUS_RENDERER_H
#define _PONTILUS_RENDERER_H

#include "rData.h"
#include "Texture.h"

namespace Pontilus
{
    namespace Renderer
    {
        enum rMode
        {
            PONT_GAME,
            PONT_POST,
            PONT_DEBUG
        };
        void start();
        void render();
        void postRender();
        void debugRender();

        //void renderRData(Graphics::rData &r, Graphics::Primitive mode, unsigned int numObjects);
        void clean();
    }
}

#endif //_PONTILUS_RENDERER_H
