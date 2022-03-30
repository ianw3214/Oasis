#pragma once
#include "events/event.hpp"

namespace Oasis
{
    enum class MouseButton : int {
        LEFT = 0,
        RIGHT,

        COUNT
    };

    /////////////////////////////////////////////////////////////////
    class MousePressedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_PRESS);

        MousePressedEvent(int x, int y, MouseButton button) 
            : mX(x), mY(y), mButton(button) {}
        int GetX() const { return mX; }
        int GetY() const { return mY; }
        MouseButton GetButton() const { return mButton; }
    private:
        int mX;
        int mY;
        MouseButton mButton;
    };

    /////////////////////////////////////////////////////////////////
    class MouseReleasedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_RELEASE);

        MouseReleasedEvent(int x, int y, MouseButton button) 
            : mX(x), mY(y), mButton(button) {}
        int GetX() const { return mX; }
        int GetY() const { return mY; }
        MouseButton GetButton() const { return mButton; }
    private:
        int mX;
        int mY;
        MouseButton mButton;
    };

    /////////////////////////////////////////////////////////////////
    class MouseMovedEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_MOVE);

        MouseMovedEvent(int x, int y, int xOffset, int yOffset) 
            : mX(x)
            , mY(y)
            , mXOffset(xOffset)
            , mYOffset(yOffset) 
        {}
        int GetX() const { return mX; }
        int GetY() const { return mY; }
        int GetXOffset() const { return mXOffset; }
        int GetYOffset() const { return mYOffset; }
    private:
        int mX;
        int mY;
        int mXOffset;
        int mYOffset;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(MOUSE_SCROLL)

        MouseScrolledEvent(int h, int v) : mHorizontalScroll(h), mVerticalScroll(v) {}
        int GetHorizontalScroll() const { return mHorizontalScroll; }
        int GetVerticalScroll() const { return mVerticalScroll; }
    private:
        int mHorizontalScroll;
        int mVerticalScroll;
    };
}