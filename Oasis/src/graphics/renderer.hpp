#pragma once

#include "util/util.hpp"

namespace Oasis
{
    class Texture;
    class Sprite;
    class AnimatedSprite;
    class Renderer
    {
    public:
        static void Init();
        static void Clear(Colour colour);

        // TODO: Make a vec class for positions
        static void DrawLine(float x1, float y1, float x2, float y2, const Colour& colour);
        static void DrawLineStrip(float * data, int num_vertices, const Colour& colour);
        static void DrawQuad(float x, float y, float w, float h, const Colour& colour, float alpha = 1.f);
        static void DrawQuad(float x, float y, float w, float h, Ref<Texture> texture);

        static void DrawSprite(const ConstRef<Sprite> sprite);
        static void DrawAnimatedSprite(Ref<AnimatedSprite> sprite);
    private:
    };
}