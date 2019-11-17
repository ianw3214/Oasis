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
        std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> m_frames;
        // Number of horizontal/vertical frames
        unsigned int m_numFramesH;
        unsigned int m_numFramesV;
        // The rate that the animation should play at
        const int kDefaultMsPerFrame = 1000 / 24;
        int m_millisecondsPerFrame;

        // Queue of animations
        std::queue<AnimationState> m_animations;

        // State variables
        std::chrono::time_point<std::chrono::system_clock> m_lastUpdate;
        unsigned int m_frameIndex;
        std::string m_currAnimation;

        // Helper function to calculate animation frame coordinates
        void UpdateSourcePosFromFrame();
    };
}