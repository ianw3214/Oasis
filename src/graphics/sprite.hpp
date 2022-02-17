#pragma once

#include <string>
#include "util/colour.hpp"

namespace Oasis
{
    class Sprite
    {
    public:
        Sprite();
        Sprite(const std::string& path);
        ~Sprite();

        void SetPos(float x, float y);
        void SetDimensions(float w, float h);
        void SetSourcePos(float x, float y);
        void SetSourceDimensions(float w, float h);
        void SetTexturePath(const std::string& path);
        void SetTint(const Oasis::Colour& colour, float alpha);

        float GetX() const { return m_x; }
        float GetY() const { return m_y; }
        float GetWidth() const { return m_width; }
        float GetHeight() const { return m_height; }
        float GetSourceX() const { return m_srcX; }
        float GetSourceY() const { return m_srcY; }
        float GetSourceWidth() const { return m_srcWidth; }
        float GetSourceHeight() const { return m_srcHeight; }
        const Oasis::Colour& GetTint() const { return m_tint; }
        float GetTintAlpha() const { return m_tintAlpha; }
        const std::string& GetTexturePath() const { return m_texturePath; }

    private:
        float m_x;
        float m_y;
        float m_width;
        float m_height;

        float m_srcX;
        float m_srcY;
        float m_srcWidth;
        float m_srcHeight;

        Oasis::Colour m_tint;
        float m_tintAlpha;

        std::string m_texturePath;
    };
}