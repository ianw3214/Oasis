#pragma once

#include <vector>
#include <functional>

#include <SDL2/SDL.h>

namespace Oasis
{
    class Event;
    class ImGuiWrapper
    {
    public:
        static void Init();
        static void Shutdown();

        static void AddWindowFunction(std::function<void()> func);

        static void OnEvent(const SDL_Event& event);
        static void Update(float deltaTime);
    private:
        static std::vector<std::function<void()>> s_windowFunctions;
    };
}