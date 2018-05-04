#include <cstdio>

#include <thread>

#include "engine/includes/QcEngine.hpp"

class testState : public State {
public:
	~testState() {}

	// TODO: separate setting state manager from init function, makes it easier on user 
	void init() {
		testTextureAnimated = new AnimatedTexture("assets/player.png");
		testTextureAnimated->generateAtlas(10, 10);
		testTextureAnimated->addAnimationState(0, 2);
		testTextureAnimated->addAnimationState(3, 5);
		testTextureAnimated->changeAnimation(1);
		test_track = loadWAV("assets/background.wav", false);
		if (test_track == -1) {
			ERR("Something went wrong when trying to load WAV file");
		}
		createFont("test", "assets/munro.ttf", 20);
	}

	void cleanup() {}
	void pause() {}
	void resume() {}

	void update() {
		if (keyPressed(SDL_SCANCODE_A)) {
			// debugging code can go here
			pauseWAV(test_track);
		}
		if (keyPressed(SDL_SCANCODE_S)) {
			playWAV(test_track);
		}
		if (keyPressed(SDL_SCANCODE_ESCAPE)) {
			exit();
		}
	}

	void render() {
		Texture testTexture("assets/test_rect.png");
		testTexture.render(0, 0);
		Texture testTexture2("assets/player.png");
		testTexture2.fullscreen = true;
		testTexture2.render();
		Texture testText(getTextTexture("test", "test", {200, 100, 100, 255}));
		testText.render(10, 10);
	}
		
private:
	AnimatedTexture * testTextureAnimated;

	WAV_track test_track;
};

int main(int argc, char* argv[]) {
	
	QcEngine::createWindow("TEST", 500, 500, false, SDL_WINDOW_BORDERLESS);
	QcEngine::setRates(60, 3);
	QcEngine::setState(std::make_unique<testState>());

	while (QcEngine::isRunning()) {
		QcEngine::update();
	}

	return 0;

}