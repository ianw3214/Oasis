#include "state.hpp"
#include "stateManager.hpp"

#include "core/engine.hpp"

void State::setManager(StateManager * manager) {
	managerRef = manager;
}

void State::setRenderer(SDL_Renderer * renderer) {
	this->renderer = renderer;
}

void State::init() {
	// do nothing lol
}

void State::setDelta(int inpDelta) {
	delta = inpDelta;
}

void State::changeState(std::unique_ptr<State> state) {
	managerRef->changeState(std::move(state));
}

void State::addState(std::unique_ptr<State> state) {
	managerRef->pushState(std::move(state));
}

void State::quitTopState() {
	managerRef->popState();
}

void State::startTextInput() {
	managerRef->startTextInput();
}

void State::stopTextInput() {
	managerRef->stopTextInput();
}

void State::resetTextInput() {
	managerRef->resetTextInput();
}

const std::string& State::getTextInput() {
	return managerRef->getTextInput();
}

void State::showCursor(bool show) const {
	SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

bool State::keyPressed(SDL_Scancode key) const {
	return managerRef->keyPressed(key);
}

bool State::keyDown(SDL_Scancode key) const {
	return managerRef->keyDown(key);
}

bool State::keyUp(SDL_Scancode key) const {
	return managerRef->keyUp(key);
}

int State::getMouseX() const {
	return managerRef->mouseX;
}

int State::getMouseY() const {
	return managerRef->mouseY;
}

bool State::leftMousePressed() const {
	return managerRef->leftMousePressed();
}

bool State::leftMouseReleased() const {
	return managerRef->leftMouseReleased();
}

bool State::leftMouseHeld() const {
	return managerRef->leftMouseHeld();
}

bool State::rightMousePressed() const {
	return managerRef->rightMousePressed();
}

bool State::rightMouseReleased() const {
	return managerRef->rightMouseReleased();
}

bool State::rightMouseHeld() const {
	return managerRef->rightMouseHeld();
}

int State::getMouseScrollUp() const {
	return managerRef->getMouseScrollUp();
}
int State::getMouseScrollDown() const {
	return managerRef->getMouseScrollDown();
}

void State::exit() {
	managerRef->quit();
}

WAV_track State::loadWAV(const std::string & path, bool loop) const {
	return QcE::get_instance()->getAudioEngine()->loadWAV(path, loop);
}

WAV_track State::loadAndPlayWAV(const std::string & path, bool loop) const {
	WAV_track track = loadWAV(path, loop);
	playWAV(track);
	return track;
}

void State::playWAV(WAV_track track) const {
	QcE::get_instance()->getAudioEngine()->play(track);
}

void State::pauseWAV(WAV_track track) const {
	QcE::get_instance()->getAudioEngine()->pause(track);
}

void State::stopWAV(WAV_track track) const {
	QcE::get_instance()->getAudioEngine()->stop(track);
}

bool drawLine(int x1, int x2, int y1, int y2) {
	return QcE::get_instance()->drawLine(x1, x2, y1, y2);
}

bool drawLine(int x1, int x2, int y1, int y2, SDL_Color color) {
	return QcE::get_instance()->drawLine(x1, x2, y1, y2, color);
}

void State::createFont(const std::string & name, const std::string & path, int size) const {
	QcE::get_instance()->getTextEngine()->createFont(name, path, size);
}

SDL_Texture * State::getTextTexture(const std::string & text, const std::string font, SDL_Color colour) const {
	return QcE::get_instance()->getTextEngine()->getTexture(text, font, colour);
}