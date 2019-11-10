#pragma once
#include "event.hpp"

namespace Oasis
{
    class TextInputEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(TEXT_INPUT);

        TextInputEvent(char * text) : m_text(text) {}
        char * GetText() const { return m_text; }
    private:
        char * m_text;
    };
}