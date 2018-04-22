#include "textureHandler.hpp"

TextureHandler::TextureHandler() {

}

TextureHandler::~TextureHandler() {
	for (auto& a : dict) {
		delete a.second;
	}
}

Texture * TextureHandler::getTexture(std::string id) {
	if (dict.find(id) == dict.end()) {
		return nullptr;
	}
	return dict[id];
}

Texture * TextureHandler::addTexture(std::string id, Texture * tex) {
	if (dict.find(id) != dict.end()) {
		delete dict[id];
	}
	return dict[id] = tex;
}

Texture * TextureHandler::addTexture(std::string id, std::string path, TextureType t) {
	if (dict.find(id) != dict.end()) {
		delete dict[id];
	}
	if (t == T_DEFAULT) {
		dict[id] = new Texture(path);
	}
	if (t == T_ANIMATED) {
		dict[id] = new AnimatedTexture(path);
	}
	return dict[id];
}
