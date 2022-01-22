/* date = January 21st 2022 4:20 pm */

#pragma once

#include <AL/al.h>
#include "ecs/AudioSource.h"

#define NUM_BUFFERS_PER_FILE 4
#define BUFFER_SIZE 65536

namespace Pontilus
{
    namespace Audio
    {
        // TODO: void getAvailableDevices();
        void initAudio();
        void updateListener();
        void addSource(Engine::ECS::AudioSource &s);
        void updateSources();
        void closeAudio();

        struct AudioFile
        {
            const char *filepath;
            void *audioData;
            ALuint buffers[NUM_BUFFERS_PER_FILE];
        };

        struct WAVFile : public AudioFile
        {
            uint8_t channels, bitsPerSample;
            int32_t sampleRate, dataSize;
            ALenum format;
        };

        int initWAVFile(WAVFile &wf, const char *filepath);
        void clearWAVFile(WAVFile &wf);
    }
}
