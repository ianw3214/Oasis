#include "sprite.hpp"
using namespace Oasis;

#include "util/util.hpp"

#include "resource/resourceManager.hpp"
#include "graphics/opengl/texture.hpp"

Sprite::Sprite()
    : m_x(0.f)
    , m_y(0.f)
    , m_width(0.f)
    , m_height(0.f)
    , m_srcX(0.f)
    , m_srcY(0.f)
    , m_srcWidth(0.f)
    , m_srcHeight(0.f)
    , m_tint(Oasis::Colours::WHITE)
    , m_tintAlpha(0.f)
    , m_texturePath("")
{

}

Sprite::Sprite(const std::string& path)
    : m_x(0.f)
    , m_y(0.f)
    , m_srcX(0.f)
    , m_srcY(0.f)
    , m_tint(Oasis::Colours::WHITE)
    , m_tintAlpha(0.f)
    , m_texturePath(path)
{
    Ref<Oasis::Texture> texture = Oasis::ResourceManager::LoadResource<Oasis::Texture>(path);
    OASIS_TRAP(texture);
    m_width = m_srcWidth = static_cast<float>(texture->getWidth());
    m_height = m_srcHeight = static_cast<float>(texture->getHeight());
}

Sprite::~Sprite()
{

}


void Sprite::SetPos(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Sprite::SetDimensions(float w, float h)
{
    m_width = w;
    m_height = h;
}

void Sprite::SetSourcePos(float x, float y)
{
    m_srcX = x;
    m_srcY = y;
}

void Sprite::SetSourceDimensions(float w, float h)
{
    m_srcWidth = w;
    m_srcHeight = h;
}

void Sprite::SetTexturePath(const std::string& path)
{
    m_texturePath = path;
}

void Sprite::SetTint(const Oasis::Colour& colour, float alpha)
{
    m_tint = colour;
    m_tintAlpha = alpha;
}