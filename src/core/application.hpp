#pragma once

#include <functional>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

// TODO: Don't use this stupid Oasis namespace lol
namespace Oasis
{
    class Event;

    ////////////////////////////////////////////////////////
    struct Configuration
    {
        int mWidth;
        int mHeight;
        const char* mName;

        const char* mStartingScene;
    };
    Configuration GetConfiguration();

    ////////////////////////////////////////////////////////
    class Application
    {
    public:
        Application(const Configuration& config);
        ~Application();

        void OnEvent(const Event& e);
        void Run();

    private:
        bool mRunning;
        double mDelta;

        int mWidth;
        int mHeight;

        SDL_Window * mWindow;
        SDL_GLContext mContext;
    private:
        friend class WindowService;
    };
}