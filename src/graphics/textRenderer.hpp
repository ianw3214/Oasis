#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "opengl/glwrappers.hpp"

#include "util/colour.hpp"
#include "util/pointer.hpp"

namespace Oasis
{
    //////////////////////////////////////////////////////////////////////////
    struct Character
    {
        Texture * mTexture;
        int mWidth;
        int mHeight;
        // Offset from baseline to top left of glyph
        int mBearingX;
        int mBearingY;
        // Offset from baseline to 
        // Offset to advance to the next glyph
        GLuint mAdvance;
    };

    //////////////////////////////////////////////////////////////////////////
    struct Font
    {
        typedef std::unordered_map<GLchar, Character> CharMap;
        CharMap mMap;
        // Store some additional info about the font
        unsigned int mSize;
    };


    const std::string kTextVertexPath = "Oasis/res/shaders/text_vertex.glsl";
    const std::string kTextFragmentPath = "Oasis/res/shaders/text_fragment.glsl";

    //////////////////////////////////////////////////////////////////////////
    class TextRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void ResetWindowDimensions();

        static void LoadFont(const std::string& name, const std::string& path, int fontSize = 16);
        static const Font& GetFont(const std::string& name);

        static void DrawCharacter(const std::string& font, GLchar character, float x, float y, const Colour& colour, Ref<Character> ch = nullptr);
        static int DrawString(const std::string& font, const std::string& str, float x, float y, const Colour& colour);
    private:

        static FT_Library s_ft;
        static std::unordered_map<std::string, Font> s_fonts;
        static Shader * s_shader;
    };
}