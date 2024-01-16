#include "audio/AudioMaster.h"
#include "audio/AudioListener.h"
#include "audio/AudioSource.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <AL/alc.h>
#include "utils/Utils.h"

namespace Pontilus
{
    namespace Audio
    {
        static ALCdevice *currentDevice;
        static ALCcontext *currentContext;

        static std::vector<WAVFile *> audioFiles;
        static std::vector<AudioSource *> audioSources;

        bool hasAudioDevice = true;

        void initAudio()
        {
            currentDevice = alcOpenDevice(nullptr);
            if (!currentDevice)
            {
                __pWarning("OpenAL could not find default audio device");
                hasAudioDevice = false;
                return;
            }

            currentContext = alcCreateContext(currentDevice, nullptr); // TODO: audio attributes
            __pAssert(currentContext, "OpenAL could not initialize audio context.");

            bool contextMadeCurrent = alcMakeContextCurrent(currentContext);
            __pAssert(contextMadeCurrent, "OpenAL could not make audio context current.");
        }

        void addSource(AudioSource &s)
        {
            audioSources.push_back(&s);
        }

        void closeAudio()
        {
            if (!__pAudioCheck) return;
            for (AudioSource *s : audioSources)
            {
                s->clear();
            }
            audioSources.clear();
            
            for (WAVFile *f : audioFiles)
            {
                clearWAVFile(*f);
            }
            audioFiles.clear();

            alcDestroyContext(currentContext);
            alcMakeContextCurrent(nullptr);
            __pAssert(alcCloseDevice(currentDevice), "Attempted to destroy uninitialized OpenGL context.");
        }

        int initWAVFile(WAVFile &wf, const char *filepath)
        {
            if (!__pAudioCheck) return -3;
            FILE *f = fopen(filepath, "r");

            if (!f)
            {
                __pWarning(".wav file %s doesn't exist.", filepath);
                return -1;
            }

            char buffer[4];

            fread(buffer, 4, 1, f);

            if (strncmp(buffer, "RIFF", 4) != 0)
            {
                __pWarning("File %s isn't a valid .wav file.", filepath);
                return -2;
            }

            wf.filepath = filepath;

            fseek(f, 4, SEEK_CUR);
            fread(buffer, 4, 1, f);

            if (strncmp(buffer, "WAVE", 4) != 0)
            {
                __pWarning("File %s isn't a valid .wav file.", filepath);
                return -2;
            }

            fseek(f, 10, SEEK_CUR);
            fread(&wf.channels, 1, sizeof(char), f);
            fseek(f, 1, SEEK_CUR);

            // bytes are in reverse order
            fread(&wf.sampleRate, 2, sizeof(char), f);
            fseek(f, 2, SEEK_CUR);

            fseek(f, 6, SEEK_CUR);
            fread(&wf.bitsPerSample, 1, sizeof(char), f);
            fseek(f, 1, SEEK_CUR);

            fread(buffer, 4, 1, f);
            if (strncmp(buffer, "data", 4) != 0)
            {
                __pWarning("File %s isn't a valid .wav file.", filepath);
                return -2;
            }

            fread(&wf.dataSize, 1, sizeof(int), f);

            wf.audioData = (char *) malloc(wf.dataSize);
            fread(wf.audioData, 1, wf.dataSize * sizeof(char), f);
            fclose(f);

            if (wf.channels == 1 && wf.bitsPerSample == 8)
                wf.format = AL_FORMAT_MONO8;
            else if (wf.channels == 1 && wf.bitsPerSample == 16)
                wf.format = AL_FORMAT_MONO16;
            else if (wf.channels == 2 && wf.bitsPerSample == 8)
                wf.format = AL_FORMAT_STEREO8;
            else if (wf.channels == 2 && wf.bitsPerSample == 16)
                wf.format = AL_FORMAT_STEREO16;
            else
            {
                wf.format = AL_FORMAT_STEREO8;
                __pWarning("Unrecognized .wav format: %d channels, %d bits per sample; defaulting to 2 and 16 respectively.", wf.channels, wf.bitsPerSample);
            }

            /*
            long numberOfSamples = f.dataSize / (f.channels * (f.bitsPerSample / 8));
            long duration = numberOfSamples / f.sampleRate; // unnecessary as of right now
             */

            __alCall(alGenBuffers, NUM_BUFFERS_PER_FILE, &wf.buffers[0]);

            audioFiles.push_back(&wf);

            return 1;
        }

        void clearWAVFile(WAVFile &wf)
        {
            if (!__pAudioCheck) return;
            if (!wf.filepath)
            {
                __pWarning("Attempted to free uninitialized WAVFile at %p.", &wf);
                return;
            }

            free(wf.audioData);
            wf.filepath = nullptr;
            wf.bitsPerSample = wf.channels = wf.dataSize = wf.sampleRate = 0;

            alDeleteBuffers(NUM_BUFFERS_PER_FILE, &wf.buffers[0]);
        }
    }
}
