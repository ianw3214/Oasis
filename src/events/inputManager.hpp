#pragma once

#include <functional>

namespace Oasis
{
    class Event;
    class InputManager
    {
    public:
        static void Init(std::function<void(Event&)> callback);

        static void Update();

        // Some inputs make more sense as state
        static bool MouseHeld() { return s_mouseDown; }
    private:
        static std::function<void(Event&)> s_eventCallback;

        // TODO: Differentiate between mouse buttons
        static bool s_mouseDown;
    };
}