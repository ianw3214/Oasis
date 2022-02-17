#pragma once
#include "events/event.hpp"

namespace Oasis
{
    ///////////////////////////////////////////////////////////////
    class KeyPressedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(KEY_PRESSED);

        KeyPressedEvent(int keyCode) : m_keyCode(keyCode) {}
        int GetKey() const { return m_keyCode; }
    private:
        int m_keyCode;
    };

    ///////////////////////////////////////////////////////////////
    class KeyReleasedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(KEY_RELEASED);

        KeyReleasedEvent(int keyCode) : m_keyCode(keyCode) {}
        int GetKey() const { return m_keyCode; }
    private:
        int m_keyCode;
    };
}