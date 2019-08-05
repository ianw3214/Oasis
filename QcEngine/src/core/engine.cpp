#include "engine.hpp"

#include "graphics/textEngine.hpp"

QcE * QcE::instance;

QcE * QcE::get_instance() {
	if (!instance) {
		instance = new QcE;
	}
	return instance;
}

Texture * QcE::getTexture(std::string id) {
	return textureHandler->getTexture(id);;
}

Texture * QcE::loadTexture(std::string id, Texture * tex) {
	return textureHandler->addTexture(id, tex);
}

Texture * QcE::loadTexture(std::string id, std::string path) {
	return textureHandler->addTexture(id, path);
}

Texture * QcE::loadTexture(std::string id, std::string path, TextureType t) {
	return textureHandler->addTexture(id, path, t);
}

bool QcE::drawLine(int x1, int y1, int x2, int y2, SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	return SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

QcE::QcE() {
	window = nullptr;
	renderer = nullptr;
	// initialize SDL libraries
	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ERR("Couldn't initialize SDL.");
		LOG(SDL_GetError());
	}
	// initialize a statemanager
	stateManager = new StateManager();
	// intiailize a texture handler
	textureHandler = new TextureHandler();
	// initialize networking system
	Network::init();
	// initialize subsystems
	audioEngine = new Audio::Engine();
	textEngine = new Text::Engine();
}

// cleanup resources in destructor
QcE::~QcE() {
	// cleanup networking library
	Network::cleanup();
	// cleanup subsystems
	delete audioEngine;
	delete textEngine;

	// delete the state manager before cleaning up SDL resources
	delete stateManager;
	delete textureHandler;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	// call the quit functions for SDL libraries
	SDL_Quit();

	// destroy CVARs
	CVAR * c_node = getCVARhead();
	while (c_node != nullptr) {
		CVAR * prev = c_node;
		c_node = c_node->next;
		delete prev;
	}
}

// basic engine methods
bool QcE::createWindow(std::string name, int width, int height, bool fullscreen, Uint32 flags) {
	if (window) SDL_DestroyWindow(window);
	if (renderer) SDL_DestroyRenderer(renderer);
	// destroy current window and renderer if they exist already
	window = SDL_CreateWindow(name.c_str(), 
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								width, 
								height, 
								fullscreen ? SDL_WINDOW_FULLSCREEN | flags : SDL_WINDOW_SHOWN | flags);
	if (!window) {
		ERR("Couldn't create an SDL window...");
		LOG(SDL_GetError());
		return false;
	}

	// also create the renderer associated with the window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (!renderer) {
		ERR("Couldn't create a Renderer for the SDL window...");
		LOG(SDL_GetError());
		return false;
	}

	// initialzie the renderer draw color to be black
	if (SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF) < 0) {
		ERR("Couldn't set render draw colour...");
		LOG(SDL_GetError());
		return false;
	}
	
	return true;
}

void QcE::setState(std::unique_ptr<State> state) {
	stateManager->changeState(std::move(state));
}

void QcE::setRates(int update, int render) {
	if (update > 0) stateManager->setUpdateRate(update);
	if (render > 0) stateManager->setRenderRate(render);
}

void QcE::update() {
	stateManager->update();
}

void QcE::render() {
	stateManager->render(renderer);
}

bool QcE::isRunning() const {
	return stateManager->isRunning();
}

SDL_Window * QcE::getWindow() const {
	return window;
}

SDL_Renderer * QcE::getRenderer() const {
	return renderer;
}
