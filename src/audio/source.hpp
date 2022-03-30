#pragma once

#include <AL/al.h>

namespace Oasis
{
    class AudioResource;
    class AudioSource
    {
    public:
        AudioSource();
        ~AudioSource();

        void Play(AudioResource * resource);
    private:
        ALuint mSource;
    };
}