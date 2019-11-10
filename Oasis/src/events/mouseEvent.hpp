#pragma once
#include "events/event.hpp"

namespace Oasis
{
    /////////////////////////////////////////////////////////////////
    class MousePressedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_PRESS);

        MousePressedEvent(int x, int y) : m_x(x), m_y(y) {}
        int GetX() const { return m_x; }
        int GetY() const { return m_y; }
    private:
        int m_x;
        int m_y;
    };

    /////////////////////////////////////////////////////////////////
    class MouseReleasedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_RELEASE);

        MouseReleasedEvent(int x, int y) : m_x(x), m_y(y) {}
        int GetX() const { return m_x; }
        int GetY() const { return m_y; }
    private:
        int m_x;
        int m_y;
    };

    /////////////////////////////////////////////////////////////////
    class MouseMovedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_MOVE);

        MouseMovedEvent(int x, int y, int xOffset, int yOffset) 
            : m_x(x)
            , m_y(y)
            , m_xOffset(xOffset)
            , m_yOffset(yOffset) 
        {}
        int GetX() const { return m_x; }
        int GetY() const { return m_y; }
        int GetXOffset() const { return m_xOffset; }
        int GetYOffset() const { return m_yOffset; }
    private:
        int m_x;
        int m_y;
        int m_xOffset;
        int m_yOffset;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_SCROLL)

        MouseScrolledEvent(int h, int v) : m_horizontalScroll(h), m_verticalScroll(v) {}
        int GetHorizontalScroll() const { return m_horizontalScroll; }
        int GetVerticalScroll() const { return m_verticalScroll; }
    private:
        int m_horizontalScroll;
        int m_verticalScroll;
    };
}