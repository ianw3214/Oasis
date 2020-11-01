#pragma once

#include "util/colour.hpp"

#include <vector>

enum class UIType
{
    NONE = 0,
    BACKGROUND,
    COUNT
};

enum class UIAnchor
{
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER,
    COUNT
};

struct UIElement
{
    unsigned int m_width, m_height;
    UIAnchor m_anchor;
    unsigned int m_xOffset, m_yOffset;
    // Each UIElement has ownership of it's children
    std::vector<UIElement*> m_children;

    // Handles the actual updating of the UI element
    UIType m_UIType;
    // Handles the data needed for the UI
    union
    {
        // Background drawing info
        struct {
            unsigned int m_borderWidth;
            Oasis::Colour m_background;
            Oasis::Colour m_border;
        };
    };
};