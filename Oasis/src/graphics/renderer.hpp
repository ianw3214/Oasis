#pragma once

class Texture;

namespace Oasis
{
    // TEMPORARY DEFINITIONS
    // TODO: REPLACE
    struct Colour
    {
        float r;
        float g;
        float b;
    };

    class Renderer
    {
    public:
        static void Init();
        static void Clear(Colour colour);

        // TODO: Make a vec class for positions
        static void DrawLine(float x1, float y1, float x2, float y2, const Colour& colour);
        static void DrawQuad(float x, float y, float w, float h, const Colour& colour);
        static void DrawQuad(float x, float y, float w, float h, const Texture& texture);
    private:
    };
}