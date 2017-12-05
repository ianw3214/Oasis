#pragma once

#include <vector>

#include "../core/texture.hpp"
#include "../math/collision.hpp"

class AnimatedTexture : public Texture {

public:
	AnimatedTexture(SDL_Texture * const texture);
	AnimatedTexture(const std::string& path);
	~AnimatedTexture();
	// also don't allow copying of animated textures
	AnimatedTexture(const AnimatedTexture&) = delete;
	AnimatedTexture& operator=(AnimatedTexture&) = delete;

	// methods to add frames to the animated texture
	void addAnimationState(int start, int end);
	void addAnimationState(std::pair<int, int> frame);
	void addToAtlas(int x, int y, int w, int h);
	void addToAtlas(Math::Rectangle rect);
	void generateAtlas(int frameWidth, int frameHeight, int num = -1);

	void changeAnimation(unsigned int anim);
	void resetAnimation();
	void render() const;
	void render(int x, int y) const;

private:

	mutable int currentFrame;
	int currentAnimation;
	std::vector<std::pair<int, int>> frames;
	std::vector<Math::Rectangle> atlas;

	// helper methods
	void renderFrame(int x, int y) const;

};