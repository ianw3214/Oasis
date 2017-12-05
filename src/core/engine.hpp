#pragma once

#include <cstdio>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "stateManager.hpp"
#include "../audio/audioEngine.hpp"
#include "../graphics/textEngine.hpp"
#include "../network/network.hpp"

// the actual engine class that holds engine data
class QcE {

public:

	static QcE * get_instance();
	~QcE();
	// delete any possible copy operations
	QcE(const QcE&) = delete;
	void operator=(const QcE&) = delete;
	
	// basic engine functionalities
	bool createWindow(std::string name, int width, int height, bool fullscreen, Uint32 flags = 0);
	void setState(std::unique_ptr<State> state);
	void setRates(int update, int render);
	
	void update();
	void render();

	bool isRunning() const;

	// accessers to base SDL objects
	SDL_Window * getWindow() const;
	SDL_Renderer * getRenderer() const;

	// access calls to subsystems
	inline Audio::Engine * getAudioEngine() {
		return audioEngine;
	}
	inline Text::Engine * getTextEngine() {
		return textEngine;
	}

private:
	QcE();
	static QcE * instance;

	// using heap allocation because we need to destroy statemanager manually before engine destructor
	StateManager * stateManager;

	// base SDL window and renderer objects
	SDL_Window * window;
	SDL_Renderer * renderer;

	// singleton subsystems (raw pointers for better control of lifetime)
	Audio::Engine * audioEngine;
	Text::Engine * textEngine;

};

// basic accessor methods
namespace QcEngine {
	inline bool createWindow(std::string name, int width, int height, bool fullscreen, Uint32 flags = 0) {
		return QcE::get_instance()->createWindow(name, width, height, fullscreen, flags);
	}

	inline void setRates(int update, int render) {
		QcE::get_instance()->setRates(update, render);
	}

	inline void setState(std::unique_ptr<State> state) {
		QcE::get_instance()->setState(std::move(state));
	}

	inline bool isRunning() {
		return QcE::get_instance()->isRunning();
	}

	inline void update() {
		QcE::get_instance()->update();
		QcE::get_instance()->render();
	}

	inline SDL_Window* getWindow() {
		return QcE::get_instance()->getWindow();
	}

	inline SDL_Renderer* getRenderer() {
		return QcE::get_instance()->getRenderer();
	}
}
