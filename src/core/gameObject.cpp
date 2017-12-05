#include "gameObject.hpp"
#include "../util/util.hpp"

GameObject::GameObject() {

}

GameObject::GameObject(Texture * texture) {
	setTexture(texture);
}

GameObject::~GameObject() {
	// delete the texture if it exists
	delete this->texture;
}

void GameObject::setTexture(Texture * texture) {
	this->texture = texture;
}

void GameObject::update(Uint32 delta) {

}

void GameObject::render(int camX, int camY) const {
	ASSERT(texture != nullptr);
	texture->render(x - camX, y - camY);
}