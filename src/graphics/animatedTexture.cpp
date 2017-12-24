#include <utility>

#include "animatedTexture.hpp"
#include "../util/util.hpp"
#include "../core/engine.hpp"

AnimatedTexture::AnimatedTexture(SDL_Texture * const texture) : Texture(texture), ms_per_frame(-1) {
	
}

AnimatedTexture::AnimatedTexture(const std::string & path) : Texture(path), ms_per_frame(-1) {
	
}

AnimatedTexture::~AnimatedTexture() {

}

void AnimatedTexture::addAnimationState(int start, int end, int next) {
	frames.push_back(std::make_pair(start, end));
	next_anim.push_back(next);
}

void AnimatedTexture::addAnimationState(std::pair<int, int> frame, int next) {
	frames.push_back(frame);
	next_anim.push_back(next);
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

void AnimatedTexture::changeFPS(int new_fps) {
	ms_per_frame = static_cast<int>(1000 / new_fps);
	timer.start();
}

void AnimatedTexture::changeAnimation(unsigned int anim) {
	if (anim >= frames.size()) {
		ERR("Tried to play an animation that is not set...");
		return;
	}
	if (currentAnimation != anim) {
		currentAnimation = anim;
		resetAnimation();
	}
}

void AnimatedTexture::resetAnimation() {
	if (frames.size() == 0) {
		currentFrame = 0;
		return;
	}
	ASSERT(static_cast<unsigned int>(currentAnimation) < frames.size());
	currentFrame = frames.at(currentAnimation).first;
	if (ms_per_frame > 0) timer.reset(true);
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
	// if we have custom fps, then break if times not up yet
	if (ms_per_frame > 0) {
		if (timer.getTicks() < static_cast<unsigned int>(ms_per_frame)) {
			return;
		}
		timer.reset(true);
	}
	// after rendering, increment the frame count
	currentFrame++;
	if (currentFrame > frames.at(currentAnimation).second) {
		// check if we have to change animations
		ASSERT(next_anim.size() == frames.size());
		if (next_anim.at(currentAnimation) >= 0)
			currentAnimation = next_anim.at(currentAnimation);
		currentFrame = frames.at(currentAnimation).first;
	}
}