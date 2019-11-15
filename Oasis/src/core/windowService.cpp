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
    return s_application->Width();
}

int WindowService::WindowHeight()
{
    return s_application->Height();
}