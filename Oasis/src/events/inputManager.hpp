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
    private:
        static std::function<void(Event&)> s_eventCallback;
    };
}