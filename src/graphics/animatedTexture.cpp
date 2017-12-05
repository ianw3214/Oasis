#include <utility>

#include "animatedTexture.hpp"
#include "../util/util.hpp"
#include "../core/engine.hpp"

AnimatedTexture::AnimatedTexture(SDL_Texture * const texture) : Texture(texture) {
	
}

AnimatedTexture::AnimatedTexture(const std::string & path) : Texture(path) {
	
}

AnimatedTexture::~AnimatedTexture() {

}

void AnimatedTexture::addAnimationState(int start, int end) {
	frames.push_back(std::make_pair(start, end));
}

void AnimatedTexture::addAnimationState(std::pair<int, int> frame) {
	frames.push_back(frame);
}

void AnimatedTexture::addToAtlas(int x, int y, int w, int h) {
	atlas.emplace_back(x, y, w, h);
}

void AnimatedTexture::addToAtlas(Math::Rectangle rect) {
	atlas.emplace_back(rect);
}

void AnimatedTexture::generateAtlas(int frameWidth, int frameHeight, int num) {
	int hNumFrames = width / frameWidth;
	int vNumFrames = height / frameHeight;
	int counter = 0;
	for (int i = 0; i < vNumFrames; ++i) {
		for (int j = 0; j < hNumFrames; ++j) {
			addToAtlas(j * frameWidth, i * frameHeight, frameWidth, frameHeight);
			// only generate up to the specified amount
			counter++;
			if (num > 0 && counter > num) {
				return;
			}
		}
	}
}

void AnimatedTexture::changeAnimation(unsigned int anim) {
	if (anim >= frames.size()) {
		ERR("Tried to play an animation that is not set...");
		return;
	}
	currentAnimation = anim;
	resetAnimation();
}

void AnimatedTexture::resetAnimation() {
	if (frames.size() == 0) {
		currentFrame = 0;
		return;
	}
	ASSERT(static_cast<unsigned int>(currentAnimation) < frames.size());
	currentFrame = frames.at(currentAnimation).first;
}

void AnimatedTexture::render() const {
	// just render the whole texture if there are no atlas rectangles
	if (atlas.size() == 0) {
		Texture::render();
	}
	// otherwise, render based on current frame and atlas
	else {
		renderFrame(0, 0);
	}
}

void AnimatedTexture::render(int x, int y) const {
	if (atlas.size() == 0) {
		Texture::render(x, y);
	}
	else {
		renderFrame(x, y);
	}
}

void AnimatedTexture::renderFrame(int x, int y) const {
	if (!texture) {
		ERR("Tried to render a NULL texture...");
		return;
	}
	// Make sure the frame we are trying to render is defined by the atlas
	ASSERT(static_cast<unsigned int>(currentFrame) < atlas.size());
	ASSERT(QcEngine::getRenderer());
	const Math::Rectangle& rect = atlas.at(currentFrame);
	SDL_Rect source = { rect.pos.x, rect.pos.y, rect.w, rect.h };
	if (fullscreen) {
		SDL_RenderCopyEx(QcEngine::getRenderer(), texture, &source, nullptr, angle, &centre, SDL_FLIP_NONE);
	}
	else {
		SDL_Rect target = { x, y, rect.w, rect.h };
		SDL_RenderCopyEx(QcEngine::getRenderer(), texture, &source, &target, angle, &centre, SDL_FLIP_NONE);
	}
	// after rendering, increment the frame count
	currentFrame++;
	if (currentFrame > frames.at(currentAnimation).second) {
		currentFrame = frames.at(currentAnimation).first;
	}
}