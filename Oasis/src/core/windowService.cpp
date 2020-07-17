#include "windowService.hpp"
using namespace Oasis;

#include "core/application.hpp"

Application * WindowService::s_application;

void WindowService::Init(Application * application)
{
    s_application = application;
}

int WindowService::WindowWidth()
{
    return s_application->m_width;
}

int WindowService::WindowHeight()
{
    return s_application->m_height;
}

SDL_Window* WindowService::GetWindow()
{
    return s_application->m_window;
}

SDL_GLContext WindowService::GetContext()
{
    return s_application->m_context;
}

float WindowService::GetDeltaF()
{
    return static_cast<float>(s_application->m_delta) / 1000.0;
}