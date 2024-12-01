#include "audio/AudioSource.h"
#include "ecs/Transform.h"
#include "audio/AudioMaster.h"

#include <AL/al.h>
#include <math.h>

namespace Pontilus
{
    namespace Audio
    {
        void AudioSource::init()
        {
            if (!__pAudioCheck) return;
            _alCall(alGenSources, 1, &this->source);
            _alCall(alSourcef, this->source, AL_PITCH, 1.0f);
            _alCall(alSourcef, this->source, AL_GAIN, 1.0f);

            Pontilus::Audio::addSource(*this);
        }

        ALuint AudioSource::alSource()
        {
            return this->source;
        }

        ALint &AudioSource::getState()
        {
            return this->state;
        }

        void AudioSource::play(Audio::WAVFile &f, bool _looping)
        {
            if (!__pAudioCheck) return;
            if (!this->parent)
            {
                _pError("AudioSource at %p has no parent.", this);
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
                    _alCall(alBufferData, f.buffers[i], f.format, f.audioData + offset, BUFFER_SIZE, f.sampleRate)
                else if (offset < f.dataSize && offset + BUFFER_SIZE > f.dataSize)
                {
                    char *data = (char *) malloc(BUFFER_SIZE);
                    memset(data, 0, BUFFER_SIZE);
                    memcpy(data, f.audioData + offset, f.dataSize - offset);
                    _alCall(alBufferData, f.buffers[i], f.format, data, BUFFER_SIZE, f.sampleRate);
                    free(data);
                }
                else
                {
                    char *data = (char *) malloc(BUFFER_SIZE);
                    memset(data, 0, BUFFER_SIZE);
                    _alCall(alBufferData, f.buffers[i], f.format, data, BUFFER_SIZE, f.sampleRate);
                    free(data);
                }
            }

            _alCall(alSourcei, this->source, AL_LOOPING, _looping);
            this->looping = _looping;
            _alCall(alSourceQueueBuffers, this->source, NUM_BUFFERS_PER_FILE, &f.buffers[0]);

            this->cursor = (int) fmin(NUM_BUFFERS_PER_FILE * BUFFER_SIZE, f.dataSize);
            this->atEnd = false;

            _alCall(alSourcePlay, this->source); // i could probably do multithreading with this

            this->state = AL_PLAYING;
            this->currentFile = &f;
        }

        // this function needs more comments
        int AudioSource::update(double _dt)
        {
            if (!__pAudioCheck) return 0;
            if (!this->parent)
            {
                _alCall(alSource3f, this->alSource(), AL_POSITION, 0.0f, 0.0f, 0.0f);
            }
            else
            {
                auto _transform = (ECS::Transform*) this->parent->getComponent(typeid(ECS::Transform));
                _alCall(alSource3f, this->alSource(), AL_POSITION, _transform->pos.x, _transform->pos.y, _transform->pos.z);
            }
            // TODO: velocity

            
            _alCall(alGetSourcei, this->alSource(), AL_SOURCE_STATE, &this->getState());

            ALint buffersProcessed = 0;
            _alCall(alGetSourcei, this->source, AL_BUFFERS_PROCESSED, &buffersProcessed);
            
            if (buffersProcessed <= 0) return 0;

            while (buffersProcessed--)
            {
                if (++buffPtr >= NUM_BUFFERS_PER_FILE) buffPtr = 0;

                ALuint buffer = this->currentFile->buffers[buffPtr];
                _alCall(alSourceUnqueueBuffers, this->source, 1, &buffer);

                if (this->state == AL_STOPPED)
                {
                    return 0;
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

                _alCall(alBufferData, buffer, this->currentFile->format, data, BUFFER_SIZE, this->currentFile->sampleRate);
                _alCall(alSourceQueueBuffers, this->source, 1, &buffer);

                free(data);
            }
            return 0;
        }

        void AudioSource::stop()
        {
            if (!__pAudioCheck) return;
            _alCall(alSourceStop, this->source);
            _alCall(alSourceUnqueueBuffers, this->source, 4, &this->currentFile->buffers[0]);

            this->currentFile = nullptr;
            this->state = AL_STOPPED;
        }

        void AudioSource::clear()
        {
            if (!__pAudioCheck) return;
            if (state == AL_PLAYING) this->stop();
            _alCall(alDeleteSources, 1, &this->source);
        }
    }
}