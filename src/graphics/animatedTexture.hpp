#pragma once

#include <vector>

#include "../core/texture.hpp"
#include "../math/collision.hpp"
#include "../util/timer.hpp"

class AnimatedTexture : public Texture {

public:
	AnimatedTexture(SDL_Texture * const texture);
	AnimatedTexture(const std::string& path);
	~AnimatedTexture();
	// also don't allow copying of animated textures
	AnimatedTexture(const AnimatedTexture&) = delete;
	AnimatedTexture& operator=(AnimatedTexture&) = delete;

	// methods to add frames to the animated texture
	void addAnimationState(int start, int end, int next = -1);
	void addAnimationState(std::pair<int, int> frame, int next = -1);
	void addToAtlas(int x, int y, int w, int h);
	void addToAtlas(Math::Rectangle rect);
	void generateAtlas(int frameWidth, int frameHeight, int num = -1);
	void changeFPS(int new_fps);

	void changeAnimation(unsigned int anim);
	void resetAnimation();

	void render() const;
	void render(int x, int y) const;

private:

	// mutable because these can be changed in the render function
	mutable int currentFrame;
	mutable int currentAnimation;

	std::vector<std::pair<int, int>> frames;
	std::vector<int> next_anim;
	std::vector<Math::Rectangle> atlas;

	int ms_per_frame;
	mutable Timer timer;

	// helper methods
	void renderFrame(int x, int y) const;

};