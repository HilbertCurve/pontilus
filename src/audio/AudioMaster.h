/* date = January 21st 2022 4:20 pm */

#pragma once

#include <AL/al.h>
#include "audio/AudioSource.h"

#define __pAudioCheck (Pontilus::Audio::hasAudioDevice)

namespace Pontilus
{
    namespace Audio
    {
        // TODO: void getAvailableDevices();
        void initAudio();
        void updateListener();
        void addSource(AudioSource &s);
        void updateSources();
        void closeAudio();

        extern bool hasAudioDevice;

        struct AudioFile
        {
            const char *filepath;
            char *audioData;
            ALuint buffers[NUM_BUFFERS_PER_FILE];
            //ALuint buffer;
        };

        struct WAVFile : public AudioFile
        {
            unsigned short int channels = 0, bitsPerSample = 0;
            int32_t sampleRate = 0, dataSize = 0;
            ALenum format;
        };

        int initWAVFile(WAVFile &wf, const char *filepath);
        void clearWAVFile(WAVFile &wf);
    }
}
