#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/texture.hpp"

namespace Text {

	/*
	Singleton Text engine class because we only need 1 per game
	*/
	class Engine {

	public:
		Engine();
		~Engine();
		// don't allow copying of the Text Engine
		Engine(const Engine& other) = delete;
		Engine operator=(const Engine& other) = delete;

		void createFont(const std::string& name, const std::string& path, int size);
		SDL_Texture * getTexture(std::string text, std::string font, SDL_Color colour);

	private:
		// store the fonts sorted by name
		std::map<std::string, TTF_Font*> fonts;

		// helper function for loading text
		SDL_Surface * loadSurfaceFromText(std::string text, SDL_Color colour, TTF_Font * gFont) const;
		SDL_Texture * loadTextureFromText(std::string text, SDL_Color colour, TTF_Font * gFont, SDL_Renderer* renderer) const;
	};

}