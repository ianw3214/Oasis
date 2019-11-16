#include "animatedSprite.hpp"
using namespace Oasis;

#include "util/util.hpp"
#include "graphics/opengl/texture.hpp"
#include "resource/resourceManager.hpp"

AnimatedSprite::AnimatedSprite()
    : Sprite()
    , m_counter(0)
    , m_frameIndex(0)
    , m_currAnimation("")
{

}

AnimatedSprite::AnimatedSprite(const std::string& path, float frame_w, float frame_h)
    : Sprite(path)
    , m_counter(0)
    , m_frameIndex(0)
    , m_currAnimation("")
{
    SetSourceDimensions(frame_w, frame_h);

    // Calculate number of sprites in sprite sheet based on texture size and input frame
    Reference<Oasis::Texture> texture = Oasis::ResourceManager::LoadResource<Oasis::Texture>(path);
    OASIS_TRAP(texture);
    m_numFramesH = texture->getWidth() / static_cast<int>(frame_w);
    m_numFramesV = texture->getHeight() / static_cast<int>(frame_h);
}

AnimatedSprite::~AnimatedSprite()
{

}


void AnimatedSprite::AddAnimation(const std::string& name, unsigned int start, unsigned int end)
{
    m_frames[name] = std::make_pair(start, end);
}

void AnimatedSprite::UpdateFrame()
{
    // Only update the frame if the timer is up
    // TODO: Use a timer instead of counting ticks
    if (m_counter < 2)
    {
        m_counter++;
    }
    else
    {
        m_counter = 0;
        m_frameIndex++;
        if (m_frameIndex > m_frames[m_currAnimation].second)
        {
            if (!m_animations.empty())
            {
                m_animations.front().loops--;
                if (m_animations.front().loops == 0)
                {
                    m_animations.pop();
                }
                if (!m_animations.empty())
                {
                    m_currAnimation = m_animations.front().animation;
                }
            }
            m_frameIndex = m_frames[m_currAnimation].first;
        }
        UpdateSourcePosFromFrame();
    }
}


void AnimatedSprite::PlayAnimation(const std::string& name, unsigned int loops)
{
    OASIS_TRAP(m_frames.find(name) != m_frames.end());

    while(!m_animations.empty()) m_animations.pop();
    QueueAnimation(name, loops);

    // Actually play the animation by setting it to the right frame
    m_currAnimation = name;
    m_frameIndex = m_frames[m_currAnimation].first;
    UpdateSourcePosFromFrame();
}

void AnimatedSprite::QueueAnimation(const std::string& name, unsigned int loops)
{
    m_animations.push(AnimationState{ name, loops });
}


void AnimatedSprite::UpdateSourcePosFromFrame()
{
    int x = m_frameIndex % m_numFramesH;
    int y = m_frameIndex / m_numFramesH;

    SetSourcePos(
        static_cast<float>(x) * GetSourceWidth(),
        static_cast<float>(y) * GetSourceHeight()
    );
}