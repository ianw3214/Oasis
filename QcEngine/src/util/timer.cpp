#include "timer.hpp"

Timer::Timer() : accumulated(0), currentStart(0) {
	// at first timer's aren't running
	running = false;
}

Timer::Timer(bool _start) : accumulated(0), currentStart(0) {
	if (_start) start();
	else running = false;
}

Timer::~Timer() {

}

void Timer::start() {
	running = true;
	currentStart = SDL_GetTicks();
}

void Timer::pause() {
	// save the current time and store it if needed
	if (running) accumulated += SDL_GetTicks() - currentStart;
	running = false;
}

void Timer::reset(bool _start) {
	// reset accumulated time
	accumulated = 0;
	// start timer again depending on input parameter
	if (_start) start();
	else running = false;
}

unsigned int Timer::getTicks() const {
	return running ? SDL_GetTicks() - currentStart + accumulated : accumulated;
}

bool Timer::paused() const {
	return !running;
}