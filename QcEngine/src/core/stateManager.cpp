#include "stateManager.hpp"
#include "state.hpp"

#include "../util/util.hpp"

StateManager::StateManager() : delta(0),
							   lastUpdate(0), 
							   lastRender(0), 
							   updateRate(DEFAULT_TICK_RATE), 
							   renderRate(DEFAULT_RENDER_RATE), 
							   running(false) {
	keyStates = SDL_GetKeyboardState(NULL);
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
	// If there's no more states, then exit the game
	if (states.size() == 0) running = false;
}

void StateManager::update() {
	// calculate the delta time
	Uint32 currentUpdate = SDL_GetTicks();
	delta = currentUpdate - lastUpdate;
	// artificially restrict delta in case time difference is too large
	delta = clamp(delta, 1, DEFAULT_DELTA_CAP);
	// get new input states
	SDL_GetMouseState(&mouseX, &mouseY);
	// poll all SDL events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		// always quit if the user presses exit
		if (e.type == SDL_QUIT) {
			running = false;
		}
		if (e.type == SDL_KEYDOWN) {
			// handle text inputs
			if (textInputting) {
				if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
					inputText.pop_back();
				}
			}
			// don't record the key down if it is already held
			if (heldKeys[e.key.keysym.scancode]) continue;
			keyPresses.push_back(e.key.keysym.scancode);
			heldKeys[e.key.keysym.scancode] = true;
		}
		if (e.type == SDL_KEYUP) {
			keyReleases.push_back(e.key.keysym.scancode);
			heldKeys[e.key.keysym.scancode] = false;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				mousePresses |= LEFT_MOUSE_PRESSED;
			}
			if (e.button.button == SDL_BUTTON_RIGHT) {
				mousePresses |= RIGHT_MOUSE_PRESSED;
			}
		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				mousePresses |= LEFT_MOUSE_RELEASED;
			}
			if (e.button.button == SDL_BUTTON_RIGHT) {
				mousePresses |= RIGHT_MOUSE_RELEASED;
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			if (e.wheel.y > 0) {
				mouseScrollUp = e.wheel.y;
			}
			if (e.wheel.y < 0) {
				mouseScrollUp = -e.wheel.y;
			}
		}
		// handle text inputs
		if (textInputting && e.type == SDL_TEXTINPUT) {
			inputText += e.text.text;
		}
	}
	if (leftMousePressed()) left_mouse_down = true;
	if (leftMouseReleased()) left_mouse_down = false;
	if (rightMousePressed()) right_mouse_down = true;
	if (rightMouseReleased()) right_mouse_down = false;
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
		mousePresses = 0;
		mouseScrollUp = 0;
		mouseScrollDown = 0;
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

bool StateManager::keyPressed(SDL_Scancode key) const {
	return keyStates[key];
}

bool StateManager::keyDown(SDL_Scancode key) const {
	return std::find(keyPresses.begin(), keyPresses.end(), key) != keyPresses.end();
}

bool StateManager::keyUp(SDL_Scancode key) const {
	return std::find(keyReleases.begin(), keyReleases.end(), key) != keyReleases.end();
}

bool StateManager::leftMousePressed() const {
	return (mousePresses & LEFT_MOUSE_PRESSED) != 0;
}

bool StateManager::leftMouseReleased() const {
	return (mousePresses & LEFT_MOUSE_RELEASED) != 0;
}

bool StateManager::leftMouseHeld() const {
	return left_mouse_down;
}

bool StateManager::rightMousePressed() const {
	return (mousePresses & RIGHT_MOUSE_PRESSED) != 0;
}

bool StateManager::rightMouseReleased() const {
	return (mousePresses & RIGHT_MOUSE_RELEASED) != 0;
}

bool StateManager::rightMouseHeld() const {
	return right_mouse_down;
}

int StateManager::getMouseScrollUp() const {
	return mouseScrollUp;
}
int StateManager::getMouseScrollDown() const {
	return mouseScrollDown;
}

void StateManager::startTextInput() {
	SDL_StartTextInput();
	textInputting = true;
}

void StateManager::stopTextInput() {
	SDL_StopTextInput();
	textInputting = false;
}

const std::string & StateManager::getTextInput() {
	return inputText;
}

void StateManager::resetTextInput() {
	inputText.clear();
}

void StateManager::setTextInput(std::string text) {
	inputText = text;
}
