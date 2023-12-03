#include "ecs/AudioListener.h"

#include "audio/AudioMaster.h"

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
            int AudioListener::update(double dt)
            {
                if (!__pAudioCheck) return 0;
                if (this->parent)
                {
                    __alCall(alListener3f, AL_POSITION, this->parent->pos.x, this->parent->pos.y, this->parent->pos.z);
                }
                else
                {
                    __alCall(alListener3f, AL_POSITION, 0.0f, 0.0f, 0.0f);
                }
                // TODO: direction and velocity

                return 0;
            }
        }
    }
}
