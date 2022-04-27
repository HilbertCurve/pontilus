/* date = November 13th 2021 10:34 am */

#pragma once

#include "graphics/rData.h"

namespace Pontilus
{
    namespace Renderer
    {
        struct PointMap
        {
            int zIndex;
            rData *dataPtr;
        };
        void initPointMap();
        void updatePointMap(double dt);
    }
}
