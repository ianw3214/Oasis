#pragma once

#include "util/colour.hpp"

#include <vector>

#include "uiFont.hpp"

// Forward declarations
namespace Oasis {
    class Sprite;
}

enum class UIType
{
    NONE = 0,
    BACKGROUND,
    TEXT,
    TEXTURE,
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
    int m_xOffset, m_yOffset;
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
        // Font drawing info
        struct {
            char * m_text;  // Need to remember to delete m_text, was allocated by serializer
            Oasis::Colour m_colour;
            UIFont m_font;
        };
        // Texture
        struct {
            char * m_path;
            // Cached texture
            Oasis::Sprite * m_cachedSprite;
        };
    };

    // Internal data for the manager to keep track of whether the UI element should be shown or not
    bool m_show;
};