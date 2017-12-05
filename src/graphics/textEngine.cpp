#include "textEngine.hpp"
#include "../util/util.hpp"
#include "../core/engine.hpp"

Text::Engine::Engine() {
	if (TTF_Init() != 0) {
		LOG("Error initializing the SDL ttf library.");
		ERR(TTF_GetError());
	}
}

Text::Engine::~Engine() {
	TTF_Quit();
}

void Text::Engine::createFont(const std::string& name, const std::string& path, int size) {
	TTF_Font * font = TTF_OpenFont(path.c_str(), size);
	if (!font) {
		ERR("Could not initialize font: " << path);
		LOG(TTF_GetError());
	}
	fonts[name] = font;
}

SDL_Texture * Text::Engine::getTexture(std::string text, std::string font, SDL_Color colour) {
	ASSERT(QcEngine::getRenderer());
	// first find the font in the list of fonts
	std::map<std::string, TTF_Font*>::iterator it;
	it = fonts.find(font);
	// if the font was not found, return a null pointer and log the error message
	if (it == fonts.end()) {
		ERR("Could not find font: " << font);
		return nullptr;
	}
	// otherwise, use the helper method to get the desired texture
	return loadTextureFromText(text, colour, (*it).second , QcEngine::getRenderer());
}

SDL_Surface * Text::Engine::loadSurfaceFromText(std::string text, SDL_Color colour, TTF_Font * gFont) const {
	SDL_Surface * textSurface = TTF_RenderText_Solid(gFont, text.c_str(), colour);
	if (!textSurface) {
		ERR("Unable to load text: " << text);
		LOG(TTF_GetError());
		return nullptr;
	}
	return textSurface;
}

SDL_Texture * Text::Engine::loadTextureFromText(std::string text, SDL_Color colour, TTF_Font * gFont, SDL_Renderer * renderer) const {
	SDL_Surface * surface = loadSurfaceFromText(text, colour, gFont);

	// convert the surface to a texture
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
	// if the texture was not loaded, throw an error and quit the function
	if (!texture) {
		ERR("Unable to load text " + text);
		LOG(SDL_GetError());
		return nullptr;
	}

	// free the temporary surface
	SDL_FreeSurface(surface);

	return texture;
}