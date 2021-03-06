#include "ecs/AudioSource.h"
#include "audio/AudioMaster.h"

#include <AL/al.h>
#include <math.h>

namespace Pontilus
{
    namespace Engine
    {
        namespace ECS
        {
            void AudioSource::init()
            {
                if (!__pAudioCheck) return;
                __alCall(alGenSources, 1, &this->source);
                __alCall(alSourcef, this->source, AL_PITCH, 1.0f);
                __alCall(alSourcef, this->source, AL_GAIN, 1.0f);

                Audio::addSource(*this);
            }

            ALuint AudioSource::alSource()
            {
                return this->source;
            }

            ALint &AudioSource::getState()
            {
                return this->state;
            }

            void AudioSource::play(Audio::WAVFile &f, bool looping)
            {
                if (!__pAudioCheck) return;
                if (!this->parent)
                {
                    __pError("AudioSource at %p has no parent.", this);
                }
                 
                if (this->state == AL_PLAYING) this->stop();

                //__alCall(alBufferData, f.buffer, f.format, f.audioData, f.dataSize, f.sampleRate);

                //__alCall(alSource3f, this->source, AL_POSITION, this->parent->pos.x, this->parent->pos.y, this->parent->pos.z);
                // TODO: velocity

                // initialize buffer
                for (int i = 0; i < NUM_BUFFERS_PER_FILE; i++)
                {
                    int offset = i * BUFFER_SIZE;

                    if (offset + BUFFER_SIZE < f.dataSize)
                        __alCall(alBufferData, f.buffers[i], f.format, f.audioData + offset, BUFFER_SIZE, f.sampleRate)
                    else if (offset < f.dataSize && offset + BUFFER_SIZE > f.dataSize)
                    {
                        char *data = (char *) malloc(BUFFER_SIZE);
                        memset(data, 0, BUFFER_SIZE);
                        memcpy(data, f.audioData + offset, f.dataSize - offset);
                        __alCall(alBufferData, f.buffers[i], f.format, data, BUFFER_SIZE, f.sampleRate);
                        free(data);
                    }
                    else
                    {
                        char *data = (char *) malloc(BUFFER_SIZE);
                        memset(data, 0, BUFFER_SIZE);
                        __alCall(alBufferData, f.buffers[i], f.format, data, BUFFER_SIZE, f.sampleRate);
                        free(data);
                    }
                }

                __alCall(alSourcei, this->source, AL_LOOPING, looping);
                this->looping = looping;
                __alCall(alSourceQueueBuffers, this->source, NUM_BUFFERS_PER_FILE, &f.buffers[0]);

                this->cursor = (int) fmin(NUM_BUFFERS_PER_FILE * BUFFER_SIZE, f.dataSize);
                this->atEnd = false;

                __alCall(alSourcePlay, this->source); // i could probably do multithreading with this

                this->state = AL_PLAYING;
                this->currentFile = &f;
            }

            // this function needs more comments
            void AudioSource::updateStream()
            {
                if (!__pAudioCheck) return;
                ALint buffersProcessed = 0;
                __alCall(alGetSourcei, this->source, AL_BUFFERS_PROCESSED, &buffersProcessed);
                
                if (buffersProcessed <= 0) return;

                while (buffersProcessed--)
                {
                    if (++buffPtr >= NUM_BUFFERS_PER_FILE) buffPtr = 0;

                    ALuint buffer = this->currentFile->buffers[buffPtr];
                    __alCall(alSourceUnqueueBuffers, this->source, 1, &buffer);

                    if (this->state == AL_STOPPED)
                    {
                        return;
                    }

                    ALsizei dataSize = BUFFER_SIZE;

                    char* data = (char *) malloc(dataSize);
                    memset(data, 0, dataSize);

                    long dataSizeToCopy = BUFFER_SIZE;
                    if(this->cursor + BUFFER_SIZE > this->currentFile->dataSize)
                        dataSizeToCopy = this->currentFile->dataSize - this->cursor;

                    if (!atEnd)
                        memcpy(data, this->currentFile->audioData + this->cursor, dataSizeToCopy);
                    this->cursor += dataSizeToCopy;

                    if (dataSizeToCopy < BUFFER_SIZE)
                    {
                        // if this occurs we're at the end of the audio data
                        this->cursor = 0;

                        // if we're looping, put start of file at the end of the most recent buffer
                        if (this->looping)
                        {
                            if (!atEnd)
                                memcpy(data + dataSizeToCopy, this->currentFile->audioData + this->cursor, BUFFER_SIZE - dataSizeToCopy);
                            this->cursor = BUFFER_SIZE - dataSizeToCopy;
                        }
                        else
                        {
                            atEnd = true; // otherwise we're at the end.
                        }
                        
                    }

                    __alCall(alBufferData, buffer, this->currentFile->format, data, BUFFER_SIZE, this->currentFile->sampleRate);
                    __alCall(alSourceQueueBuffers, this->source, 1, &buffer);

                    free(data);
                }
            }

            void AudioSource::stop()
            {
                if (!__pAudioCheck) return;
                __alCall(alSourceStop, this->source);
                __alCall(alSourceUnqueueBuffers, this->source, 4, &this->currentFile->buffers[0]);

                this->currentFile = nullptr;
                this->state = AL_STOPPED;
            }

            void AudioSource::clear()
            {
                if (!__pAudioCheck) return;
                if (state == AL_PLAYING) this->stop();
                __alCall(alDeleteSources, 1, &this->source);
            }
        }
    }
}