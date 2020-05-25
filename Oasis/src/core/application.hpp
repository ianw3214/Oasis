#pragma once

#include <functional>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace Oasis
{
    class Event;
    class IState;

    ////////////////////////////////////////////////////////
    struct Configuration
    {
        int m_width;
        int m_height;
        const char * m_name;

        std::function<IState*()> m_initState;
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
        bool m_running;

        int m_width;
        int m_height;

        SDL_Window * m_window;
        SDL_GLContext m_context;
    private:
        friend class WindowService;
    };
}