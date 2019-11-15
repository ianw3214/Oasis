#pragma once

namespace Oasis
{
    class Application;
    class WindowService
    {
    public:
        static void Init(Application * application);
        
        static int WindowWidth();
        static int WindowHeight();
    private:
        static Application * s_application;
    };
}