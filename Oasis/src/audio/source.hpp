#pragma once

#include <AL/al.h>

namespace Oasis
{
    class AudioSource
    {
    public:
        AudioSource();
        ~AudioSource();

        void Play(ALuint buffer);
    private:
        ALuint m_source;
    };
}