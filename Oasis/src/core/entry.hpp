#include "application.hpp"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
extern Oasis::Configuration Oasis::GetConfiguration();
INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    
    Oasis::Application application(Oasis::GetConfiguration());
    application.Run();

    return 0;
}
#endif