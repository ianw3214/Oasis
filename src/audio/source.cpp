#include "source.hpp"
using namespace Oasis;

#include "util/trap.hpp"

#include "audio/audio.hpp"

AudioSource::AudioSource()
{
    alGenSources(1, &mSource);
    alSourcef(mSource, AL_GAIN, 1.f);
    alSourcef(mSource, AL_PITCH, 1.f);
    alSource3f(mSource, AL_POSITION, 0.f, 0.f, 0.f);
    alSource3f(mSource, AL_VELOCITY, .0f, .0f, .0f);
}

AudioSource::~AudioSource()
{
    alDeleteSources(1, &mSource);
}

void AudioSource::Play(AudioResource * resource)
{
    alSourcei(mSource, AL_BUFFER, resource->GetBuffer());
    alSourcePlay(mSource);

    ALenum error = alGetError();
    OASIS_TRAP(error == AL_NO_ERROR);
}