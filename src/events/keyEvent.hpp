#pragma once
#include "events/event.hpp"

namespace Oasis
{
    ///////////////////////////////////////////////////////////////
    class KeyPressedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(KEY_PRESSED);

        KeyPressedEvent(int keyCode) : mKeyCode(keyCode) {}
        int GetKey() const { return mKeyCode; }
    private:
        int mKeyCode;
    };

    ///////////////////////////////////////////////////////////////
    class KeyReleasedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(KEY_RELEASED);

        KeyReleasedEvent(int keyCode) : mKeyCode(keyCode) {}
        int GetKey() const { return mKeyCode; }
    private:
        int mKeyCode;
    };
}