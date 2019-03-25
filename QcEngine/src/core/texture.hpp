#pragma once

#include <string>

#include <SDL2/SDL.h>

// TODO: move colour struct definition elsewhere
struct Colour {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	// leave the default constructor to be a white colour
	Colour() : r(0xFF), g(0xFF), b(0xFF) {}
	Colour(Uint8 _r, Uint8 _g, Uint8 _b) : r(_r), g(_g), b(_b) {}
};

class Texture {

public:
	Texture(SDL_Texture * const texture);
	Texture(const std::string& path);
	virtual ~Texture();
	// don't allow copying, rendering of the same texture should use the same object
	Texture(const Texture&) = delete;
	Texture& operator=(Texture&) = delete;

	// getter methods
	int getWidth() const;
	int getHeight() const;

	// setter methods
	void setCentre(int x, int y);
	void setAngle(double angle);
	void setBlendMode(SDL_BlendMode blending);
	void setColourModulation(const Colour& c);
	void setAlpha(Uint8 alpha);
	void setHorizontalFlip(bool flip);
	void setVerticalFlip(bool flip);

	// basic texture attributes
	bool fullscreen;

	virtual void render() const;
	virtual void render(int x, int y) const;

protected:
	// variables that hold information about the texture
	int width, height;
	SDL_Point centre;
	double angle;
	SDL_RendererFlip flip;

	SDL_Texture * texture;

	// SDL wrapper functions
	bool loadTexture(const std::string& path, SDL_Renderer* renderer);
};