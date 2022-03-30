#include "animatedSprite.hpp"
using namespace Oasis;

#include "util/util.hpp"
#include "graphics/opengl/texture.hpp"
#include "resource/resourceManager.hpp"

AnimatedSprite::AnimatedSprite()
    : Sprite()
    , mMillisecondsPerFrame(kDefaultMsPerFrame)
    , mLastUpdate(std::chrono::system_clock::now())
    , mFrameIndex(0)
    , mCurrAnimation("")
{

}

AnimatedSprite::AnimatedSprite(const std::string& path, float frame_w, float frame_h)
    : Sprite(path)
    , mMillisecondsPerFrame(kDefaultMsPerFrame)
    , mLastUpdate(std::chrono::system_clock::now())
    , mFrameIndex(0)
    , mCurrAnimation("")
{
    SetSourceDimensions(frame_w, frame_h);

    // Calculate number of sprites in sprite sheet based on texture size and input frame
    Ref<Oasis::Texture> texture = Oasis::ResourceManager::LoadResource<Oasis::Texture>(path);
    OASIS_TRAP(texture);
    mNumFramesH = texture->getWidth() / static_cast<int>(frame_w);
    mNumFramesV = texture->getHeight() / static_cast<int>(frame_h);
}

AnimatedSprite::~AnimatedSprite()
{

}

void AnimatedSprite::SetFPS(int fps)
{
    mMillisecondsPerFrame = 1000 / fps;
}

void AnimatedSprite::AddAnimation(const std::string& name, unsigned int start, unsigned int end)
{
    mFrames[name] = std::make_pair(start, end);
}

void AnimatedSprite::UpdateFrame()
{
    // Only update the frame if the timer is up
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - mLastUpdate);
    if (duration.count() >= mMillisecondsPerFrame)
    {
        mLastUpdate = std::chrono::system_clock::now();
        mFrameIndex++;
        if (mFrameIndex > mFrames[mCurrAnimation].second)
        {
            if (!mAnimations.empty())
            {
                mAnimations.front().loops--;
                if (mAnimations.front().loops == 0)
                {
                    mAnimations.pop();
                }
                if (!mAnimations.empty())
                {
                    mCurrAnimation = mAnimations.front().animation;
                }
            }
            mFrameIndex = mFrames[mCurrAnimation].first;
        }
        UpdateSourcePosFromFrame();
    }
}


void AnimatedSprite::PlayAnimation(const std::string& name, unsigned int loops)
{
    OASIS_TRAP(mFrames.find(name) != mFrames.end());

    while(!mAnimations.empty()) mAnimations.pop();
    QueueAnimation(name, loops);

    // Actually play the animation by setting it to the right frame
    mCurrAnimation = name;
    mFrameIndex = mFrames[mCurrAnimation].first;
    UpdateSourcePosFromFrame();

    mLastUpdate = std::chrono::system_clock::now();
}

void AnimatedSprite::QueueAnimation(const std::string& name, unsigned int loops)
{
    mAnimations.push(AnimationState{ name, loops });
}


void AnimatedSprite::UpdateSourcePosFromFrame()
{
    int x = mFrameIndex % mNumFramesH;
    int y = mFrameIndex / mNumFramesH;

    SetSourcePos(
        static_cast<float>(x) * GetSourceWidth(),
        static_cast<float>(y) * GetSourceHeight()
    );
}