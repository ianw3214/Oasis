#pragma once

#include "texture.hpp"
#include "../graphics/animatedTexture.hpp"
#include "../graphics/tilemap.hpp"

#include <map>
#include <string>

enum TextureType {
	T_DEFAULT,
	T_ANIMATED,
	T_TILEMAP
};

class TextureHandler {

public:
	TextureHandler();
	~TextureHandler();
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler operator=(const TextureHandler&) = delete;

	Texture* getTexture(std::string id);
	Texture* addTexture(std::string id, Texture* tex);
	Texture* addTexture(std::string id, std::string path, TextureType t = T_DEFAULT);

private:
	std::map<std::string, Texture*> dict;
};