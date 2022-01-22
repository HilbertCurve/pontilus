/* date = January 21st 2022 5:27 pm */

#pragma once

#include <AL/al.h>
#include "ecs/Component.h"

#define NUM_BUFFERS_PER_FILE 4
#define BUFFER_SIZE 65536

namespace Pontilus
{
    namespace Audio { struct WAVFile; }
    namespace Engine
    {
        namespace ECS
        {
            
            
            class AudioSource : public Component
            {
                public:
                AudioSource();
                void play(Audio::WAVFile &f, bool looping);
                void updateStream();
                void stop();
                ALuint alSource();
                ALint getState();
                void clear();

                private:
                ALuint source;
                ALint state = AL_STOPPED;
                long cursor = BUFFER_SIZE * NUM_BUFFERS_PER_FILE;
                Audio::WAVFile *currentFile;
            };
        }
    }
}
