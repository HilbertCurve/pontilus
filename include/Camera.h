/* date = August 30th 2021 9:08 pm */

#ifndef _PONTILUS_CAMERA_H
#define _PONTILUS_CAMERA_H

#include "Math.h"

namespace Application
{
    namespace Renderer
    {
        struct Camera
        {
            Math::Vec3 position = Math::Vec3{0, 0, 0};
            // Vec3{pitch (x-axis), yaw (y-axis), roll (z-axis)}
            Math::Vec3 rotation = Math::Vec3{0, 0, 0};
            float fov = 90; // is this necessary? i'd like to implement it at some point.
            
            Math::Mat4& getProjection();
            Math::Mat4& getView();
            
            // these should NEVER be accessed directly (unless you know what you're doing)
            Math::Mat4 projection;
            Math::Mat4 view;
        };
        
        void update(Camera& c);
    }
}

#endif //_PONTILUS_CAMERA_H
