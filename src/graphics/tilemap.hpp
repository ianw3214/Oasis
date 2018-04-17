#pragma once

#include <vector>

#include "../core/texture.hpp"
#include "../math/collision.hpp"

class TileMap : public Texture {

public:
	TileMap(SDL_Texture * const texture);
	TileMap(const std::string& path);
	~TileMap();
	// also don't allow copying of animated textures
	TileMap(const TileMap&) = delete;
	TileMap& operator=(TileMap&) = delete;

	void generateTiles(int tile_width, int tile_height);

	void render(int tile_num = 0) const;
	void render(int x, int y, int tile_num = 0) const;

// private:
	std::vector<Math::Rectangle> tiles;

};