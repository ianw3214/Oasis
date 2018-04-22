#include "tilemap.hpp"

#include "../core/engine.hpp"
#include "../util/util.hpp"

TileMap::TileMap(SDL_Texture * const texture) : Texture(texture) {

}

TileMap::TileMap(const std::string& path) : Texture(path) {

}

TileMap::~TileMap() {

}

void TileMap::generateTiles(int tile_width, int tile_height) {
	tiles.clear();
	for (int i = 0; i < (height / tile_height); ++i) {
		for (int j = 0; j < (width / tile_width); ++j) {
			tiles.push_back(Math::Rectangle(j * tile_width, i * tile_height, tile_width, tile_height));
		}
	}
	// if there are no tiles generated, just use the texture as a single whole tile
	if (tiles.size() == 0) {
		tiles.push_back(Math::Rectangle(0, 0, width, height));
	}
}

int TileMap::getNumTiles() const {
	return tiles.size();
}

void TileMap::render(int tile_num) const {
	render(0, 0, tile_num);
}

void TileMap::render(int x, int y, int tile_num) const {
	ASSERT(QcEngine::getRenderer());
	if (!texture) {
		ERR("Tried to render a NULL texture...");
		return;
	}
	// print a warning if there's no rect set for the specified tile
	if (static_cast<unsigned int>(tile_num) >= tiles.size()) {
		ERR("TILE: " << tile_num << " IS NOT SPECIFIED IN THE TILEMAP");
		tile_num = 0;
	}
	SDL_Rect src = { tiles[tile_num].pos.x, tiles[tile_num].pos.y, tiles[tile_num].w, tiles[tile_num].h };
	if (fullscreen) {
		SDL_RenderCopyEx(QcEngine::getRenderer(), texture, &src, nullptr, angle, &centre, flip);
	} else {
		SDL_Rect target = { x, y, tiles[tile_num].w, tiles[tile_num].h };
		SDL_RenderCopyEx(QcEngine::getRenderer(), texture, &src, &target, angle, &centre, flip);
	}
}