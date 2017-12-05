#pragma once

#include <string>

#include "core/texture.hpp"

class GameObject {
public:
	GameObject();
	GameObject(Texture * texture);
	virtual ~GameObject();

	void setTexture(Texture* texture);

	virtual void update(Uint32 delta);
	virtual void render(int camX = 0, int camY = 0) const;

	int x, y;

protected:

	// let whatever subclass handle what the texture will be like
	Texture * texture;
	
};