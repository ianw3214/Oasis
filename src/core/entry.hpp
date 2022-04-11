#include "application.hpp"

#ifdef PLATFORmWindowS
#include <windows.h>
INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    
    Oasis::Application application(Oasis::GetConfiguration());
    application.Run();

    return 0;
}
#endif