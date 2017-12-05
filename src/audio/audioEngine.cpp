#include <thread>

#include <SDL.h>

#include "audioEngine.hpp"
#include "../util/util.hpp"

Audio::Engine::Engine() {
	if (!init()) {
		ERR("Error initializint audio engine...");
	}
}

Audio::Engine::~Engine() {
	alcCloseDevice(device);
}

WAV_track Audio::Engine::loadWAV(const std::string& path, bool loop) {
	// create a source for the sound to laod from
	ALuint source = 0;
	alGenSources((ALuint)1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, .0f, .0f, .0f);
	alSource3f(source, AL_VELOCITY, .0f, .0f, .0f);
	if (loop) alSourcei(source, AL_LOOPING, 1);
	else alSourcei(source, AL_LOOPING, 0);
	// check for errors
	ALCenum error = alGetError();
	if (error != AL_NO_ERROR) {
		ERR("Something went wrong with OpenAL...");
	}

	// start loading the actual WAV file
	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 * wav_buffer;

	// SDL helper method to load WAV file
	if (SDL_LoadWAV(path.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL) {
		ERR("Could not load wave file: " << path);
		LOG(SDL_GetError());
		return -1;
	}

	// load the WAV into a buffer
	ALuint buffer;
	alGenBuffers((ALuint)1, &buffer);
	// convert SDL WAV format to openAL WAV format
	ALenum format;
	// stereo
	if (wav_spec.channels == 2) {
		// 8 bit
		if (wav_spec.format == AUDIO_S8 || wav_spec.format == AUDIO_U8)
			format = AL_FORMAT_STEREO8;
		// 16 bit
		else 
			format = AL_FORMAT_STEREO16;
	}
	// mono
	else {
		// 8 bit
		if (wav_spec.format == AUDIO_S8 || wav_spec.format == AUDIO_U8)
			format = AL_FORMAT_MONO8;
		// 16 bit
		else
			format = AL_FORMAT_MONO16;
	}
	
	// fill buffer with data and bind the buffer to a source
	alBufferData(buffer, format, wav_buffer, wav_length, wav_spec.freq);
	alSourcei(source, AL_BUFFER, buffer);
	
	// create a thread for playing the WAV track
	pause(source);
	std::thread playThread(&Engine::playWAV, this, source);
	playThread.detach();

	return source;
}

void Audio::Engine::play(WAV_track source) {
	// don't play again if it's already playing
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	if (source_state != AL_PLAYING) {
		alSourcePlay(source);
	}
}

void Audio::Engine::pause(WAV_track source) {
	alSourcePause(source);
}

void Audio::Engine::stop(WAV_track source) {
	alSourceStop(source);
}

void Audio::Engine::setVolume(WAV_track source, float volume) {
	alSourcef(source, AL_GAIN, volume);
}

bool Audio::Engine::init() {
	device = alcOpenDevice(nullptr);
	if (!device) {
		ERR("Couldn't open an audio device...");
		return false; 
	}
	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context)) {
		ERR("Failed to make OpenAL context current...");
		return false;
	}
	return true;
}

// member function to be called by a thread that will play a WAV file until it is done
void Audio::Engine::playWAV(ALuint source) {
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	while (source_state == AL_STOPPED) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	}
}