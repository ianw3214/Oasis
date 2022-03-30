#pragma once

#include <functional>

#include "mouseEvent.hpp"

namespace Oasis
{
    class Event;
    class InputManager
    {
    public:
        static void Init(std::function<void(Event&)> callback);

        static void Update();

        // Some inputs make more sense as state
        static bool MouseHeld(MouseButton button);
    private:
        // TODO: consider removing callback and storing state instead
        static std::function<void(Event&)> s_eventCallback;

        static bool s_mouseDown[MouseButton::COUNT];
    };
}