#pragma once

#include <vector>
#include <memory>

#include <SDL.h>

#include "state.hpp"

#define DEFAULT_TICK_RATE 60
#define DEFAULT_RENDER_RATE 60

using pState = std::unique_ptr<State>;

class StateManager {

public:

	StateManager();
	~StateManager();

	void setUpdateRate(int rate);
	void setRenderRate(int rate);

	void changeState(pState state);
	void pushState(pState state);
	void popState();

	void update();
	void render(SDL_Renderer* renderer) const;

	bool isRunning() const;
	void quit();

	// input handling variables
	int mouseX, mouseY;
	// returns true if the key is held
	bool keyPressed(SDL_Scancode key) const;
	// returns true if the key is pressed at the very frame
	bool keyDown(SDL_Scancode key) const;
	// returns true if the key is released at the very frame
	bool keyUp(SDL_Scancode key) const;
	bool getMousePressed() const;
	bool getMouseRelease() const;
	bool mouse_down;
	// TODO: change format of mouse events
	SDL_MouseButtonEvent mousePressedEvent;
	SDL_MouseButtonEvent mouseReleaseEvent;

private:

	// keep a stack of states
	std::vector<pState> states;
		
	// keep track of delta times
	mutable Uint32 delta, lastUpdate, lastRender;
	int updateRate, renderRate;

	bool running;

	// input handling variables
	const Uint8* keyStates;
	std::vector<SDL_Scancode> keyPresses;
	std::vector<SDL_Scancode> keyReleases;
	bool mousePressed;
	bool mouseRelease;

	// other SDL things
	SDL_Renderer * renderer;

};