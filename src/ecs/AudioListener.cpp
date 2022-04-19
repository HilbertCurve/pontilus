#include "ecs/AudioListener.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            AudioListener &AudioListener::get()
            {
                static AudioListener instance = AudioListener();
                return instance;
            }
        }
    }
}
