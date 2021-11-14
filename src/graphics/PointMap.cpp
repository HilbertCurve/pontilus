#include "postprocessing/PointMap.h"
#include "Application.h"

namespace Pontilus
{
    namespace Graphics
    {
        void initPointMap(PointMap &pm)
        {
            static int z;
            pm.dataPtr = &pointLightPool;
            pm.zIndex = z++;
        }

        void updatePointMap(PointMap &pm)
        {
            ((float *)pm.dataPtr->data)[0] += 0.001f;
        }
    }
}