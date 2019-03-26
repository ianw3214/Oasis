#include "core/engine.hpp"

class TestState : public State {
public:
    
    ~TestState() {}

    void init() {
        test_animatedTexture = new AnimatedTexture("res/animate.png");
        test_animatedTexture->generateAtlas(64, 80);
        test_animatedTexture->addAnimationState(0, 1);
        test_animatedTexture->changeAnimation(0);
        test_track = loadWAV("res/test.wav", false);
        if (!test_track) {
            ERR("Something went wrong when trying to load WAV File");
        }
        createFont("test", "res/Munro.ttf", 20);
    }

    void cleanup() {}
    void pause() {}
    void resume() {}

    void update() {
        if (keyPressed(SDL_SCANCODE_ESCAPE)) {
            exit();
        }
        if (keyPressed(SDL_SCANCODE_S)) {
            playWAV(test_track);
        }
        if (keyPressed(SDL_SCANCODE_A)) {
            pauseWAV(test_track);
        }
    }

    void render() {
        // Render normal textures
        Texture test("res/animate.png");
        test.render(100, 100);
        // Render animated textures
        test_animatedTexture->render(0, 0);
        // Render text
        Texture testText(getTextTexture("test", "test", {200, 100, 100, 255}));
        testText.render(200, 200);
    }

private:
    AnimatedTexture * test_animatedTexture;
    WAV_track test_track;
};

int main(int argc, char* argv[]) {

    QcEngine::createWindow("TEST", 500, 500, false, SDL_WINDOW_BORDERLESS);
    QcEngine::setRates(60, 3);
    QcEngine::setState(std::make_unique<TestState>());

    while(QcEngine::isRunning()) {
        QcEngine::update();
    }

    return 0;
}