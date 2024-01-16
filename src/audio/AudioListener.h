/* date = January 21st 2022 4:24 pm */

#pragma once

#include "ecs/Component.h"

namespace Pontilus
{
    namespace Audio
    {
        class AudioListener : public ECS::Component
        {
            public:
            static AudioListener &get();
            virtual int update(double dt);
            virtual bool isSingleton() { return true; }
            private:
            AudioListener() = default;
            virtual ~AudioListener() = default;
        };
    }
}
