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
    return s_application->mWidth;
}

int WindowService::WindowHeight()
{
    return s_application->mHeight;
}

SDL_Window* WindowService::GetWindow()
{
    return s_application->mWindow;
}

SDL_GLContext WindowService::GetContext()
{
    return s_application->mContext;
}

float WindowService::GetDeltaF()
{
    return static_cast<float>(s_application->mDelta) / 1000.0f;
}

void WindowService::SetWindowDimensions(int width, int height)
{
    s_application->mWidth = width;
    s_application->mHeight = height;
}