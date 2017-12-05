#pragma once

#include <string>

#include <AL/al.h>
#include <AL/alc.h>

using WAV_track = ALuint;

namespace Audio {

	/*
		Singleton Audio engine class because we only need 1 per game
	*/
	class Engine {

	public:
		Engine();
		~Engine();
		// don't allow copying of engines
		Engine(const Engine& other) = delete;
		Engine operator=(const Engine& other) = delete;

		WAV_track loadWAV(const std::string& path, bool loop);

		void play(WAV_track source);
		void pause(WAV_track source);
		void stop(WAV_track source);

		void setVolume(WAV_track source, float volume);
	private:

		ALCdevice * device;
		ALCcontext * context;

		bool init();
		void playWAV(ALuint source);
	};

}