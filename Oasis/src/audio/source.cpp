#include "source.hpp"
using namespace Oasis;

#include "util/trap.hpp"

#include "audio/audio.hpp"

AudioSource::AudioSource()
{
    alGenSources(1, &m_source);
    alSourcef(m_source, AL_GAIN, 1.f);
    alSourcef(m_source, AL_PITCH, 1.f);
    alSource3f(m_source, AL_POSITION, 0.f, 0.f, 0.f);
    alSource3f(m_source, AL_VELOCITY, .0f, .0f, .0f);
}

AudioSource::~AudioSource()
{
    alDeleteSources(1, &m_source);
}

void AudioSource::Play(AudioResource * resource)
{
    alSourcei(m_source, AL_BUFFER, resource->GetBuffer());
    alSourcePlay(m_source);

    ALenum error = alGetError();
    OASIS_TRAP(error == AL_NO_ERROR);
}