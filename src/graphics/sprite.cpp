#include "sprite.hpp"
using namespace Oasis;

#include "util/util.hpp"

#include "resource/resourceManager.hpp"
#include "graphics/opengl/texture.hpp"

Sprite::Sprite()
    : mX(0.f)
    , mY(0.f)
    , mWidth(0.f)
    , mHeight(0.f)
    , mSrcX(0.f)
    , mSrcY(0.f)
    , mSrcWidth(0.f)
    , mSrcHeight(0.f)
    , mTint(Oasis::Colours::WHITE)
    , mTintAlpha(0.f)
    , mTexturePath("")
{

}

Sprite::Sprite(const std::string& path)
    : mX(0.f)
    , mY(0.f)
    , mSrcX(0.f)
    , mSrcY(0.f)
    , mTint(Oasis::Colours::WHITE)
    , mTintAlpha(0.f)
    , mTexturePath(path)
{
    Ref<Oasis::Texture> texture = Oasis::ResourceManager::LoadResource<Oasis::Texture>(path);
    OASIS_TRAP(texture);
    mWidth = mSrcWidth = static_cast<float>(texture->getWidth());
    mHeight = mSrcHeight = static_cast<float>(texture->getHeight());
}

Sprite::~Sprite()
{

}


void Sprite::SetPos(float x, float y)
{
    mX = x;
    mY = y;
}

void Sprite::SetDimensions(float w, float h)
{
    mWidth = w;
    mHeight = h;
}

void Sprite::SetSourcePos(float x, float y)
{
    mSrcX = x;
    mSrcY = y;
}

void Sprite::SetSourceDimensions(float w, float h)
{
    mSrcWidth = w;
    mSrcHeight = h;
}

void Sprite::SetTexturePath(const std::string& path)
{
    mTexturePath = path;
}

void Sprite::SetTint(const Oasis::Colour& colour, float alpha)
{
    mTint = colour;
    mTintAlpha = alpha;
}