#include "audio.hpp"
using namespace Oasis;

#include <SDL2/SDL.h>

#include "util/trap.hpp"

ALCdevice * AudioEngine::s_device;
ALCcontext * AudioEngine::s_context;

Resource * AudioResource::Load(const std::string& path)
{
    return new AudioResource(path);
}

AudioResource::AudioResource(const std::string& path)
{
    // SDL helper method to load WAV file
    SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 * wav_buffer;
	SDL_AudioSpec * res = SDL_LoadWAV(path.c_str(), &wav_spec, &wav_buffer, &wav_length);
	OASIS_TRAP(res);

    // convert SDL WAV format to openAL WAV format
	ALenum format;
	// stereo
	if (wav_spec.channels == 2) 
    {
        format = wav_spec.format == AUDIO_S8 || wav_spec.format == AUDIO_U8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	}
	// mono
	else 
    {
        format = wav_spec.format == AUDIO_S8 || wav_spec.format == AUDIO_U8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	}

    alGenBuffers(1, &m_buffer);
    alBufferData(m_buffer, format, wav_buffer, wav_length, wav_spec.freq);

    SDL_FreeWAV(wav_buffer);

    ALenum error = alGetError();
    OASIS_TRAP(error == AL_NO_ERROR);
}

AudioResource::~AudioResource()
{
    alDeleteBuffers(1, &m_buffer);
}

void AudioEngine::Init()
{
    s_device = alcOpenDevice(nullptr);
    OASIS_TRAP(s_device);
    s_context = alcCreateContext(s_device, nullptr);
    ALCboolean result = alcMakeContextCurrent(s_context);
    OASIS_TRAP(result == ALC_TRUE);
}

void AudioEngine::Shutdown()
{
    
}

void AudioEngine::SetListenerData()
{
    alListener3f(AL_POSITION, 0.f, 0.f, 0.f);
    alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
}