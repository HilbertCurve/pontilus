#include "utils/Utils.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "audio/AudioMaster.h" // for .wav utils

int main()
{
    using namespace Pontilus;
    Audio::initAudio();

    Audio::WAVFile f;
    Audio::initWAVFile(f, "./assets/sounds/test1.wav");

    printf("%d, %d, %d, %d\n", f.channels, f.bitsPerSample, f.sampleRate, f.dataSize);
    //__alCall(alBufferData, f.buffer, f.format, f.audioData, f.dataSize, f.sampleRate);

    ALuint source;
    __alCall(alGenSources, 1, &source);
    __alCall(alSourcef, source, AL_PITCH, 1.0f);
    __alCall(alSourcef, source, AL_GAIN, 1.0f);
    __alCall(alSource3f, source, AL_POSITION, 0.0f, -100.0f, 0.0f);
    __alCall(alSource3f, source, AL_VELOCITY, 0.0f, 10.0f, 0.0f);
    __alCall(alSourcei, source, AL_LOOPING, false);

    __alCall(alSourceQueueBuffers, source, NUM_BUFFERS_PER_FILE, &f.buffers[0]);

    __alCall(alSourcePlay, source);

    ALint state = AL_PLAYING;

    long cursor = NUM_BUFFERS_PER_FILE * BUFFER_SIZE;

    while (state == AL_PLAYING)
    {
        {
            static int totalBuffersProcessed = 0;

            ALint buffersProcessed = 0;
            __alCall(alGetSourcei, source, AL_BUFFERS_PROCESSED, &buffersProcessed);

            if (buffersProcessed > 0)
            while (buffersProcessed--)
            {
                totalBuffersProcessed++;

                ALuint buffer;
                __alCall(alSourceUnqueueBuffers, source, 1, &buffer);

                ALsizei dataSize = BUFFER_SIZE;
                
                char *data = (char *) malloc(dataSize);
                memset(data, 0, dataSize);

                size_t dataSizeToCopy = BUFFER_SIZE;
                if (cursor + BUFFER_SIZE > f.dataSize)
                    dataSizeToCopy = f.dataSize - cursor;
                
                memcpy(data, f.audioData + cursor, dataSizeToCopy);
                cursor += dataSizeToCopy;

                if (dataSizeToCopy < BUFFER_SIZE)
                {
                    cursor = 0;
                    memcpy(data + dataSizeToCopy, f.audioData + cursor, BUFFER_SIZE - dataSizeToCopy);
                    cursor = BUFFER_SIZE - dataSizeToCopy;
                }
                __alCall(alBufferData, buffer, f.format, data, BUFFER_SIZE, f.sampleRate);
                __alCall(alSourceQueueBuffers, source, 1, &buffer);

                free(data);
            }
        }
        __alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    }

    Audio::closeAudio();
    Audio::clearWAVFile(f);

    __alCall(alDeleteSources, 1, &source);

    return 0;
}