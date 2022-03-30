#pragma once

#include <string>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

#include "resource/resource.hpp"

namespace Oasis
{
    //////////////////////////////////////////////////////////////////
    class AudioResource : public Resource
    {
    public:
        static Resource * Load(const std::string& path);

        AudioResource(const std::string& path);
        ~AudioResource();

        ALuint GetBuffer() const { return mBuffer; }
    private:
        ALuint mBuffer;
    };

    //////////////////////////////////////////////////////////////////
    class AudioEngine
    {
    public:
        static void Init();
        static void Shutdown();

        static void SetListenerData();
    private:
        static ALCdevice * s_device;
        static ALCcontext * s_context;
    };
}