#pragma once

#include "events/event.hpp"

namespace Oasis
{
    class WindowCloseEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(WINDOW_CLOSE);
    };
}