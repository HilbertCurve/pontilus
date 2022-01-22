#include "audio/AudioMaster.h"
#include "ecs/AudioListener.h"
#include "ecs/AudioSource.h"

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

        static std::vector<WAVFile> audioFiles;
        static std::vector<Engine::ECS::AudioSource> audioSources;

        void initAudio()
        {
            currentDevice = alcOpenDevice(nullptr);
            __pAssert(currentDevice, "OpenAL could not default find audio device.");

            currentContext = alcCreateContext(currentDevice, nullptr); // TODO: audio attributes
            __pAssert(currentContext, "OpenAL could not initialize audio context.");

            bool contextMadeCurrent = alcMakeContextCurrent(currentContext);
            __pAssert(contextMadeCurrent, "OpenAL could not make audio context current.");
        }

        void updateListener()
        {
            static auto &l = Engine::ECS::AudioListener::get();
            alListener3f(AL_POSITION, l.parent->pos.x, l.parent->pos.y, l.parent->pos.z);
            // TODO: direction and velocity
        }

        void addSource(Engine::ECS::AudioSource &s)
        {
            audioSources.push_back(s);
        }

        void updateSources()
        {
            for (Engine::ECS::AudioSource &s : audioSources)
            {
                alSource3f(s.alSource(), AL_POSITION, s.parent->pos.x, s.parent->pos.y, s.parent->pos.z);
                if (s.getState() == AL_PLAYING)
                    s.updateStream();
                // TODO: velocity
            }
        }

        void closeAudio()
        {
            for (WAVFile &f : audioFiles)
            {
                clearWAVFile(f);
            }
            audioFiles.clear();

            for (auto &s : audioSources)
            {
                s.clear();
            }
            audioSources.clear();

            alcDestroyContext(currentContext);
            alcMakeContextCurrent(nullptr);
            __pAssert(alcCloseDevice(currentDevice), "Attempted to destroy uninitialized OpenGL context.");
        }

        int initWAVFile(WAVFile &wf, const char *filepath)
        {
            FILE *f = fopen(filepath, "r");

            if (!f)
            {
                __pWarning(".wav file %s doesn't exist.", filepath);
                return 0;
            }

            char buffer[4];

            fread(buffer, 1, 4 * sizeof(char), f);

            if (strncmp(buffer, "RIFF", 4) != 0)
            {
                __pWarning("File %s isn't a valid .wav file.", filepath);
                return 0;
            }

            wf.filepath = filepath;

            fseek(f, 4, SEEK_CUR);
            fread(buffer, 1, 4 * sizeof(char), f);

            if (strncmp(buffer, "WAVE", 4) != 0)
            {
                __pWarning("File %s isn't a valid .wav file.", filepath);
                return 0;
            }

            fseek(f, 10, SEEK_CUR);
            fread(&wf.channels, 2, sizeof(char), f);
            fread(&wf.sampleRate, 4, sizeof(char), f);

            fseek(f, 6, SEEK_CUR);
            fread(&wf.bitsPerSample, 2, sizeof(char), f);

            fread(buffer, 1, 4 * sizeof(char), f);
            if (strncmp(buffer, "data", 4) != 0)
            {
                __pWarning("File %s isn't a valid .wav file.", filepath);
                return 0;
            }

            fread(&wf.dataSize, 4, sizeof(char), f);

            wf.audioData = malloc(wf.dataSize);
            fread(&wf.audioData, 1, wf.dataSize * sizeof(char), f);
            fclose(f);

            ALenum format;

            if (wf.channels == 1 && wf.bitsPerSample == 8)
                format = AL_FORMAT_MONO8;
            else if (wf.channels == 1 && wf.bitsPerSample == 16)
                format = AL_FORMAT_MONO16;
            else if (wf.channels == 2 && wf.bitsPerSample == 8)
                format = AL_FORMAT_STEREO8;
            else if (wf.channels == 2 && wf.bitsPerSample == 16)
                format = AL_FORMAT_STEREO16;
            else
            {
                format = AL_FORMAT_MONO8;
                __pWarning("Unrecognized .wav format: %d channels, %d bits per sample; defaulting to 2 and 8 respectively.", wf.channels, wf.bitsPerSample);
            }

            /*
            long numberOfSamples = f.dataSize / (f.channels * (f.bitsPerSample / 8));
            long duration = numberOfSamples / f.sampleRate; // unnecessary as of right now
                */

            alGenBuffers(NUM_BUFFERS_PER_FILE, &wf.buffers[0]);
            audioFiles.push_back(wf);

            return 1;
        }

        void clearWAVFile(WAVFile &wf)
        {
            if (!wf.filepath)
            {
                __pWarning("Attempted to free uninitialized WAVFile at %p.", &wf);
                return;
            }

            free(wf.audioData);
            wf.filepath = nullptr;
            wf.bitsPerSample = wf.channels = wf.dataSize = wf.sampleRate = 0;

            alDeleteBuffers(4, &wf.buffers[0]);
        }
    }
}
