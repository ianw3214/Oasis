#pragma once

#include <string>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

namespace Oasis
{
    class AudioEngine
    {
    public:
        static void Init();
        static void Shutdown();

        static void SetListenerData();

        static int LoadSound(const std::string& path);

    private:
        static std::vector<ALuint> s_buffers;
        static ALCdevice * s_device;
        static ALCcontext * s_context;
    };
}