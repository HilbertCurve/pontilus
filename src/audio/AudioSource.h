/* date = January 21st 2022 5:27 pm */

#pragma once

#include <AL/al.h>
#include "ecs/Component.h"

#define NUM_BUFFERS_PER_FILE 4
#define BUFFER_SIZE 65536

namespace Pontilus
{
    namespace Audio
    {
        struct WAVFile;
        class AudioSource : public ECS::Component
        {
            public:
            void init();
            void play(Audio::WAVFile &f, bool looping);
            virtual int update(double dt);
            virtual void clear();
            void stop();
            ALuint alSource();
            ALint &getState();

            private:
            ALuint source;
            ALint state = AL_STOPPED;
            long cursor = BUFFER_SIZE * NUM_BUFFERS_PER_FILE;
            bool looping;
            // internal use only.
            bool atEnd = false;
            int buffPtr = 0;
            Audio::WAVFile *currentFile;
        };
    }
}
