#include "audio/AudioListener.h"
#include "ecs/Transform.h"
#include "audio/AudioMaster.h"

namespace Pontilus
{
    namespace Audio
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
                ECS::Transform *_t = (ECS::Transform *)this->parent->getComponent(typeid(ECS::Transform));
                __alCall(alListener3f, AL_POSITION, _t->pos.x, _t->pos.y, _t->pos.z);
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
