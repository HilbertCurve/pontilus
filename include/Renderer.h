/* date = August 26th 2021 8:40 pm */

#ifndef _PONTILUS_RENDERER_H
#define _PONTILUS_RENDERER_H

#include "Camera.h"
#include "Rend.h"

namespace Pontilus
{
    namespace Renderer
    {
        
        //void addBatch(Graphics::RenderBatch &b);
        void start();
        void render();
        void addRend(Graphics::Rend &r);
        void clean();
    }
}

#endif //_PONTILUS_RENDERER_H
