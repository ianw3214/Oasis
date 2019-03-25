#include "core/engine.hpp"

class TestState : public State {
public:
    
    ~TestState() {}

    void init() {

    }

    void cleanup() {}
    void pause() {}
    void resume() {}

    void update() {
        if (keyPressed(SDL_SCANCODE_ESCAPE)) {
            exit();
        }
    }

    void render() {
        
    }

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