#pragma once

#include <functional>

#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

// TODO: Don't use this stupid Oasis namespace lol
namespace Oasis
{
    class Event;
    class System;
    class Component;

    ////////////////////////////////////////////////////////
    struct Configuration
    {
        int mWidth;
        int mHeight;
        const char* mName;

        const char* mStartingScene;

        // TODO: There has got to be a better way of doing this
        std::vector<System*> mSystems;
        std::function<void()> mInitExternalComponents;
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