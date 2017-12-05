#include "stateManager.hpp"
#include "state.hpp"

#include "../util/util.hpp"

StateManager::StateManager() : delta(0),
							   lastUpdate(0), 
							   lastRender(0), 
							   updateRate(DEFAULT_TICK_RATE), 
							   renderRate(DEFAULT_RENDER_RATE), 
							   running(false) {
	
}

StateManager::~StateManager() {
	for (pState& state : states) {
		state->cleanup();
	}
}

void StateManager::setUpdateRate(int rate) {
	updateRate = rate;
}

void StateManager::setRenderRate(int rate) {
	renderRate = rate;
}

void StateManager::changeState(pState state) {
	// expect the caller to use std::move in the input state
	states.clear();
	state->setManager(this);
	pushState(std::move(state));
	// the game is running if we push a new state, so set running to true
	running = true;
}

void StateManager::pushState(pState state) {
	pState newptr;
	std::swap(newptr, state);
	// initialize the new state before pushing it to the stack
	newptr->setManager(this);
	newptr->setRenderer(renderer);
	newptr->init();
	states.push_back(std::move(newptr));
	// the game is running if we push a new state, so set running to true
}

void StateManager::popState() {
	if (states.size() == 0) return;
	pState state = std::move(states.at(states.size() - 1));
	// cleanup the state before popping it from the stack
	state->cleanup();
	states.pop_back();
}

void StateManager::update() {
	// calculate the delta time
	Uint32 currentUpdate = SDL_GetTicks();
	delta = currentUpdate - lastUpdate;
	// get new input states
	keyStates = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&mouseX, &mouseY);
	// poll all SDL events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		// always quit if the user presses exit
		if (e.type == SDL_QUIT) {
			running = false;
		}
		if (e.type == SDL_KEYDOWN) {
			keyPresses.push_back(e.key.keysym.scancode);
		}
		if (e.type == SDL_KEYUP) {
			keyReleases.push_back(e.key.keysym.scancode);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			mousePressed = true;
			mousePressedEvent = e.button;
		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			mouseRelease = true;
			mouseReleaseEvent = e.button;
		}
	}
	// only update if we have passed enough time
	if (delta > static_cast<unsigned int>(1.f / updateRate * 1000.f)) {
		// update all the states
		for (pState& state : states) {
			state->setDelta(delta);
			state->update();
		}
		// change lastUpdate so we can calculate delta next frame
		lastUpdate = currentUpdate;
		// clear the key press/release states
		keyPresses.clear();
		keyReleases.clear();
		mousePressed = false;
		mouseRelease = false;
	}
}

void StateManager::render(SDL_Renderer * renderer) const {
	// don't do anything if we're not running anymore
	if (!running) return;
	// calculate elapsed time
	Uint32 timeDiff = SDL_GetTicks() - lastRender;
	// only render if enough time has passed
	if (timeDiff > static_cast<unsigned int>(1.f / renderRate * 1000.f)) {
		SDL_RenderClear(renderer);
		for (const pState& state : states) {
			state->render();
		}
		lastRender = SDL_GetTicks();
		SDL_RenderPresent(renderer);
	}
}

bool StateManager::isRunning() const {
	return running;
}

void StateManager::quit() {
	running = false;
}

bool StateManager::keyPressed(SDL_Scancode key) {
	return keyStates[key];
}

bool StateManager::keyDown(SDL_Scancode key) const {
	return std::find(keyPresses.begin(), keyPresses.end(), key) != keyPresses.end();
}

bool StateManager::keyUp(SDL_Scancode key) const {
	return std::find(keyReleases.begin(), keyReleases.end(), key) != keyReleases.end();
}

bool StateManager::getMousePressed() const {
	return mousePressed;
}

bool StateManager::getMouseRelease() const {
	return mouseRelease;
}
