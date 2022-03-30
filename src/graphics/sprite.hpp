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

        float GetX() const { return mX; }
        float GetY() const { return mY; }
        float GetWidth() const { return mWidth; }
        float GetHeight() const { return mHeight; }
        float GetSourceX() const { return mSrcX; }
        float GetSourceY() const { return mSrcY; }
        float GetSourceWidth() const { return mSrcWidth; }
        float GetSourceHeight() const { return mSrcHeight; }
        const Oasis::Colour& GetTint() const { return mTint; }
        float GetTintAlpha() const { return mTintAlpha; }
        const std::string& GetTexturePath() const { return mTexturePath; }

    private:
        float mX;
        float mY;
        float mWidth;
        float mHeight;

        float mSrcX;
        float mSrcY;
        float mSrcWidth;
        float mSrcHeight;

        Oasis::Colour mTint;
        float mTintAlpha;

        std::string mTexturePath;
    };
}