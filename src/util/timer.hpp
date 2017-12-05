#pragma once

#include <SDL.h>

#define TIMER_START true
#define TIMER_IDLE false

class Timer {

public:
	Timer();
	Timer(bool _start);
	~Timer();

	void start();
	void pause();
	void reset(bool _start);

	unsigned int getTicks() const;
	bool paused() const;
private:
	bool running;
	Uint32 accumulated;
	Uint32 currentStart;
};