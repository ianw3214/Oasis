#pragma once

#include <vector>
#include <memory>
#include <string>

#include <SDL2/SDL.h>

#include "state.hpp"

#define DEFAULT_TICK_RATE	60
#define DEFAULT_RENDER_RATE 60

#define DEFAULT_DELTA_CAP	100

#define LEFT_MOUSE_PRESSED		(1 << 0)
#define LEFT_MOUSE_RELEASED		(1 << 1)
#define RIGHT_MOUSE_PRESSED		(1 << 2)
#define RIGHT_MOUSE_RELEASED	(1 << 3)

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
	// mouse press functions
	bool leftMousePressed() const;
	bool leftMouseReleased() const;
	bool leftMouseHeld() const;
	bool rightMousePressed() const;
	bool rightMouseReleased() const;
	bool rightMouseHeld() const;
	int getMouseScrollUp() const;
	int getMouseScrollDown() const;

	// text input handling variables
	void startTextInput();
	void stopTextInput();
	const std::string& getTextInput();
	void resetTextInput();
	void setTextInput(std::string text);

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
	std::string inputText;

	// TODO: think of a better solution for key down events
	bool heldKeys[256];
	bool left_mouse_down;
	bool right_mouse_down;
	char mousePresses;
	bool textInputting;
	int mouseScrollUp;
	int mouseScrollDown;

	// other SDL things
	SDL_Renderer * renderer;

};