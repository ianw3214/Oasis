#pragma once

#define EVENT_CLASS_TYPE(type)  static EventType GetTypeStatic() { return EventType::type; }\
							    virtual EventType GetType() const override { return GetTypeStatic(); }

namespace Oasis
{
    ////////////////////////////////////////////
    enum class EventType
    {
        NONE = 0,

        // WINDOW EVENTS
        WINDOW_CLOSE,

        // KEY EVENTS
        KEY_PRESSED,
        KEY_RELEASED,

        // MOUSE EVENTS
        MOUSE_PRESSED,
        MOUSE_MOVED
    };

    ////////////////////////////////////////////
    class Event
    {
    public:
        // Allow events to return static typing information
        virtual EventType GetType() const = 0;
    };
}