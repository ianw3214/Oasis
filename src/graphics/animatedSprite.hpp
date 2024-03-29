#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <queue>
#include <chrono>

#include "sprite.hpp"

namespace Oasis
{
    //////////////////////////////////////////////
    struct AnimationState
    {
        std::string animation;
        unsigned int loops;
    };

    //////////////////////////////////////////////
    class AnimatedSprite : public Sprite
    {
    public:
        AnimatedSprite();
        AnimatedSprite(const std::string& path, float frame_w, float frame_h);
        ~AnimatedSprite();

        void SetFPS(int fps);

        void AddAnimation(const std::string& name, unsigned int start, unsigned int end);
        void UpdateFrame();

        void PlayAnimation(const std::string& name, unsigned int loops = 1);
        void QueueAnimation(const std::string& name, unsigned int loops = 1);
    private:
        // Animated Sprite information
        std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> mFrames;
        // Number of horizontal/vertical frames
        unsigned int mNumFramesH;
        unsigned int mNumFramesV;
        // The rate that the animation should play at
        const int kDefaultMsPerFrame = 1000 / 24;
        int mMillisecondsPerFrame;

        // Queue of animations
        std::queue<AnimationState> mAnimations;

        // State variables
        std::chrono::time_point<std::chrono::system_clock> mLastUpdate;
        unsigned int mFrameIndex;
        std::string mCurrAnimation;

        // Helper function to calculate animation frame coordinates
        void UpdateSourcePosFromFrame();
    };
}