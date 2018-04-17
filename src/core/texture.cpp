#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../util/util.hpp"
#include "../core/engine.hpp"

Texture::Texture(SDL_Texture * const texture) : texture(texture) {
	if (texture != nullptr) SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	flip = SDL_FLIP_NONE;
}

Texture::Texture(const std::string& path) {
	ASSERT(QcEngine::getRenderer());
	if (!loadTexture(path, QcEngine::getRenderer())) {
		return;
	}
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	flip = SDL_FLIP_NONE;
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

// getter methods

int Texture::getWidth() const {
	return width;
}

int Texture::getHeight() const {
	return height;
}

// setter methods

void Texture::setCentre(int x, int y) {
	centre = { x, y };
}

void Texture::setAngle(double angle) {
	this->angle = angle;
}

void Texture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(texture, blending);
}

void Texture::setColourModulation(const Colour& c) {
	SDL_SetTextureColorMod(texture, c.r, c.g, c.b);
}

void Texture::setAlpha(Uint8 alpha) {
	if (SDL_SetTextureAlphaMod(texture, alpha) < 0)
		ERR("Unable to set alpha of texture: " << SDL_GetError());
}

void Texture::setHorizontalFlip(bool flip) {
	if (flip) {
		this->flip = static_cast<SDL_RendererFlip>((int)(this->flip) | SDL_FLIP_HORIZONTAL);
	}
	else {
		this->flip = static_cast<SDL_RendererFlip>((int)(this->flip) & ~SDL_FLIP_HORIZONTAL);
	}
}

void Texture::setVerticalFlip(bool flip) {
	if (flip) {
		this->flip = static_cast<SDL_RendererFlip>((int)(this->flip) | SDL_FLIP_VERTICAL);
	}
	else {
		this->flip = static_cast<SDL_RendererFlip>((int)(this->flip) & ~SDL_FLIP_VERTICAL);
	}
}

void Texture::render() const {
	ASSERT(QcEngine::getRenderer());
	render(0, 0);
}

void Texture::render(int x, int y) const {
	ASSERT(QcEngine::getRenderer());
	if (!texture) {
		ERR("Tried to render a NULL texture...");
		return;
	}
	if (fullscreen) {
		SDL_RenderCopyEx(QcEngine::getRenderer(), texture, nullptr, nullptr, angle, &centre, flip);
	}
	else {
		SDL_Rect target = { x, y, width, height };
		SDL_RenderCopyEx(QcEngine::getRenderer(), texture, nullptr, &target, angle, &centre, flip);
	}
}

bool Texture::loadTexture(const std::string& path, SDL_Renderer* renderer) {

	/* OLD SDL_IMAGE WAY OF DOING THINGS

	// Load the surface first
	SDL_Surface * loadSurface = nullptr;

	// load the image at the specified path onto the surface
	loadSurface = IMG_Load(path.c_str());
	// if the image was not loaded, throw an error and quit the function
	if (!loadSurface) {
		ERR("Unable to load image: " + path);
		LOG(IMG_GetError());
		return false;
	}
	
	*/

	// NEW IMAGE LOADING WITH STB_IMAGE
	int req_format = STBI_rgb_alpha;
	int original_format;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &original_format, req_format);
	if (data == NULL) {
		ERR("Couldn't load image: " << path);
		LOG(stbi_failure_reason());
		return false;
	}

	SDL_Surface * loadSurface = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height, 32, 4 * width, SDL_PIXELFORMAT_RGBA32);
	if (!loadSurface) {
		ERR("Unable to load image: " << path);
		LOG(SDL_GetError());
		return false;
	}

	// convert the surface to a texture
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, loadSurface);
	// if the texture was not loaded, throw an error and quit the function
	if (!texture) {
		ERR("Unable to load surface " + path);
		LOG(SDL_GetError());
		return false;
	}

	// free the temporary data
	SDL_FreeSurface(loadSurface);
	stbi_image_free(data);

	this->texture = texture;

	return true;

}
