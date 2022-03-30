#pragma once
#include "event.hpp"

namespace Oasis
{
    class TextInputEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(TEXT_INPUT);

        TextInputEvent(char * text) : mText(text) {}
        char * GetText() const { return mText; }
    private:
        char * mText;
    };
}