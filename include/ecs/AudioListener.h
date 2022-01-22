/* date = January 21st 2022 4:24 pm */

#pragma once

#include "ecs/Component.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            class AudioListener : public Component
            {
                public:
                static AudioListener &get();
                private:
                AudioListener() = default;
            };
        }
    }
}
