#pragma once

#include <vector>

enum class UIType
{
    NONE = 0,
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
};