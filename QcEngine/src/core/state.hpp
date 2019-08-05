#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "audio/audioEngine.hpp"
#include "textureHandler.hpp"

class StateManager;

class State {

public:
	State() {}
	virtual ~State() {}

	void setManager(StateManager * manager);
	void setRenderer(SDL_Renderer * renderer);
	virtual void init();
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;
	
	void setDelta(int inpDelta);
	virtual void update() = 0;
	virtual void render() = 0;

	void changeState(std::unique_ptr<State> state);
	void addState(std::unique_ptr<State> state);
	void quitTopState();

	// TODO: This should probably be private but I'm lazy
	void startTextInput();
	void stopTextInput();
	void resetTextInput();
	const std::string& getTextInput();

protected:

	// some basic methods to interface SDL
	void showCursor(bool show) const;
	bool keyPressed(SDL_Scancode key) const;
	// these two functions return true if the events happened on that exact frame
	bool keyDown(SDL_Scancode key) const;
	bool keyUp(SDL_Scancode key) const;
	int getMouseX() const;
	int getMouseY() const;
	bool leftMousePressed() const;
	bool leftMouseReleased() const;
	bool leftMouseHeld() const;
	bool rightMousePressed() const;
	bool rightMouseReleased() const;
	bool rightMouseHeld() const;
	int getMouseScrollUp() const;
	int getMouseScrollDown() const;

	// some basic methods to interface state manager
	int delta;
	void exit();

	// music/sound utility methods
	WAV_track loadWAV(const std::string& path, bool loop = false) const;
	WAV_track loadAndPlayWAV(const std::string& path, bool loop = false) const;
	void playWAV(WAV_track track) const;
	void pauseWAV(WAV_track track) const;
	void stopWAV(WAV_track track) const;

	// Rendering methods
	bool drawLine(int x1, int x2, int y1, int y2);
	bool drawLine(int x1, int x2, int y1, int y2, SDL_Color color);

	// font/text utility methods
	void createFont(const std::string& name, const std::string& path, int size) const;
	SDL_Texture * getTextTexture(const std::string& text, const std::string font, SDL_Color colour) const;

	// keep a reference of the state manager (and some other SDL stuff)
	StateManager * managerRef;
	SDL_Renderer * renderer;

};