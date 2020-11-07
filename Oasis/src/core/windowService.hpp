#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

namespace Oasis
{
    class Application;
    class WindowService
    {
    public:
        static void Init(Application * application);
        
        static int WindowWidth();
        static int WindowHeight();

        static SDL_Window* GetWindow();
        static SDL_GLContext GetContext();

        static float GetDeltaF();
    private:
        static Application * s_application;

        // Functions that only certain classes should have access to
        // TODO: Is this the best way of handling this?
        friend class InputManager;
        static void SetWindowDimensions(int width, int height);
    };
}