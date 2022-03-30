#pragma once

#include "util/colour.hpp"

#include <vector>
#include <string>

#include "uiFont.hpp"

// Forward declarations
namespace Oasis {
    class Sprite;
    class AnimatedSprite;
}

enum class UIType
{
    NONE = 0,
    BACKGROUND,
    TEXT,
    TEXTURE,
    TEXT_DYNAMIC,
    BUTTON,
    ANIMATED_TEXTURE,
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
    unsigned int mWidth, mHeight;
    UIAnchor mAnchor;
    int mXOffset, mYOffset;
    // Each UIElement has ownership of it's children
    std::vector<UIElement*> mChildren;

    // Handles the actual updating of the UI element
    UIType mUIType;
    // Handles the data needed for the UI
    union
    {
        // Background drawing info
        struct {
            unsigned int mBorderWidth;
            Oasis::Colour mBackground;
            Oasis::Colour mBorder;
        };
        // Text drawing info
        struct {
            char * mText;  // Need to remember to delete mText, was allocated by serializer
            Oasis::Colour mColour;
            UIFont mFont;
        };
        // Texture
        struct {
            char * mPath;
            // Cached texture
            Oasis::Sprite * mCachedSprite;
        };
        // Dynamic text drawing info
        struct {
            char * mFormatString;
            Oasis::Colour mColour;
            UIFont mFont;
        };
        // Button
        struct {
            char * mPath;
            char * mHoverPath;
            char * mClickEvent;
            // Cached textures
            Oasis::Sprite * mCachedButtonSprite;
            Oasis::Sprite * mCachedHoverSprite;
            // Internal state for the engine to keep track of
            bool mHovering;
        };
        // Animated Texture
        struct {
            char * mPath;
            int mFrameWidth;
            int mFrameHeight;
            int mAnimFrames;
            int mFps;
            // Cached texture
            Oasis::AnimatedSprite * mCachedAnimatedSprite;
        };
    };

    // Internal data for the manager to keep track of whether the UI element should be shown or not
    bool mShow;
};