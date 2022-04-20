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
        static bool MouseClicked(MouseButton button);
        static bool MouseHeld(MouseButton button);
        static int MouseX() { return sMouseX; }
        static int MouseY() { return sMouseY; }
    private:
        // TODO: consider removing callback and storing state instead
        static std::function<void(Event&)> s_eventCallback;

        static bool sMouseClicked[MouseButton::COUNT];
        static bool sMouseHeld[MouseButton::COUNT];
        static int sMouseX, sMouseY;
    };
}