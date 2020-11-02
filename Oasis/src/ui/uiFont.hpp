#pragma once

#include <string>

enum class UIFont
{
    DEFAULT = 0,
    SMALL,
    NONE
};
inline int GetUIFontSize(UIFont font)
{
    if (font == UIFont::DEFAULT) return 32;
    if (font == UIFont::SMALL) return 16;
    if (font == UIFont::NONE) return 0;
    return 0;
}
inline std::string GetUIFont(UIFont font)
{
    if (font == UIFont::DEFAULT) return "default";
    if (font == UIFont::SMALL) return "small";
    if (font == UIFont::NONE) return "none";
    return "";
}
inline std::string GetUIFontPath(UIFont font)
{
    if (font == UIFont::DEFAULT) return "res/fonts/ProggyClean.ttf";
    if (font == UIFont::SMALL) return "res/fonts/ProggyClean.ttf";
    if (font == UIFont::NONE) return "";
    return "";
}