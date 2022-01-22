#include "ecs/AudioSource.h"
#include "audio/AudioMaster.h"

#include "AL/al.h"

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            AudioSource::AudioSource()
            {
                alGenSources(1, &this->source);
                alSourcef(this->source, AL_PITCH, 1.0f);
                alSourcef(this->source, AL_GAIN, 1.0f);
            }

            ALuint AudioSource::alSource()
            {
                return this->source;
            }

            ALint AudioSource::getState()
            {
                return this->state;
            }

            void AudioSource::play(Audio::WAVFile &f, bool looping)
            {
                if (!this->parent)
                {
                    __pError("AudioSource at %p has no parent.", this);
                }

                for(int i = 0; i < NUM_BUFFERS_PER_FILE; ++i)
                {
                    alBufferData(f.buffers[i], f.format, f.audioData + i * BUFFER_SIZE, BUFFER_SIZE, f.sampleRate);
                }

                alSource3f(this->source, AL_POSITION, this->parent->pos.x, this->parent->pos.y, this->parent->pos.z);
                // TODO: velocity
                alSourcei(this->source, AL_LOOPING, looping);
                alSourceQueueBuffers(this->source, NUM_BUFFERS_PER_FILE, &f.buffers[0]);
                alSourcePlay(this->source); // i could probably do multithreading with this

                this->state = AL_PLAYING;
            }

            void AudioSource::updateStream()
            {
                ALint buffersProcessed = 0;
                alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

                if (buffersProcessed <= 0)
                    return;

                while (buffersProcessed--)
                {
                    ALuint buffer;
                    alSourceUnqueueBuffers(source, 1, &buffer);

                    ALsizei dataSize = BUFFER_SIZE;

                    char* data = (char *) malloc(dataSize);
                    std::memset(data, 0, dataSize);

                    std::size_t dataSizeToCopy = BUFFER_SIZE;
                    if(cursor + BUFFER_SIZE > this->currentFile->dataSize)
                        dataSizeToCopy = this->currentFile->dataSize - cursor;

                    std::memcpy(&data[0], this->currentFile->audioData + cursor, dataSizeToCopy);
                    cursor += dataSizeToCopy;

                    if(dataSizeToCopy < BUFFER_SIZE)
                    {
                        cursor = 0;
                        std::memcpy(&data[dataSizeToCopy], this->currentFile->audioData + cursor, BUFFER_SIZE - dataSizeToCopy);
                        cursor = BUFFER_SIZE - dataSizeToCopy;
                    }

                    alBufferData(buffer, this->currentFile->format, data, BUFFER_SIZE, this->currentFile->sampleRate);
                    alSourceQueueBuffers(source, 1, &buffer);

                    free(data);
                }
                alGetSourcei(this->source, AL_SOURCE_STATE, &this->state);
            }

            void AudioSource::stop()
            {
                alSourceStop(this->source);

                this->state = AL_STOPPED;
            }

            void AudioSource::clear()
            {
                alDeleteSources(1, &this->source);
            }
        }
    }
}