#include "Camera.h"
#include "Math.h"
#include "Application.h"

using namespace Application::Math;

namespace Application
{
    namespace Renderer
    {
        Mat4& Camera::getProjection()
        {
            return this->projection.setOrtho(-20.0f,
                                             20.0f,
                                             -10.5f,
                                             10.5f,
                                             0.0f, 100.0f);
        }
        
        Mat4& Camera::getView()
        {
            Vec3 cameraEye = this->position;
            Vec3 cameraFront = this->position + Vec3{0.0f, 0.0f, -1.0f};
            Vec3 cameraUp = Vec3{0.0f, 1.0f, 0.0f}; //TODO(HilbertCurve): incorporate camera rotation.
            
            return this->view.setLookAt(cameraEye, cameraFront, cameraUp);
        }
    }
}
