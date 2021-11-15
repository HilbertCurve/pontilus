#include "postprocessing/PointMap.h"

#include <glm/glm.hpp>

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
            
        }
    }
}