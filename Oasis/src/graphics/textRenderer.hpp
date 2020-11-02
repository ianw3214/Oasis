#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "opengl/glwrappers.hpp"

#include "util/colour.hpp"

namespace Oasis
{
    //////////////////////////////////////////////////////////////////////////
    struct Character
    {
        Texture * m_texture;
        int m_width;
        int m_height;
        // Offset from baseline to top left of glyph
        int m_bearingX;
        int m_bearingY;
        // Offset from baseline to 
        // Offset to advance to the next glyph
        GLuint m_advance;
    };

    //////////////////////////////////////////////////////////////////////////
    struct Font
    {
        typedef std::unordered_map<GLchar, Character> CharMap;
        CharMap m_map;
        // Store some additional info about the font
        unsigned int m_size;
    };


    const std::string kTextVertexPath = "res/shaders/text_vertex.glsl";
    const std::string kTextFragmentPath = "res/shaders/text_fragment.glsl";

    //////////////////////////////////////////////////////////////////////////
    class TextRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void LoadFont(const std::string& name, const std::string& path, int fontSize = 16);
        static const Font& GetFont(const std::string& name);

        static void DrawCharacter(const std::string& font, GLchar character, float x, float y, const Colour& colour);
        static int DrawString(const std::string& font, const std::string& str, float x, float y, const Colour& colour);
    private:

        static FT_Library s_ft;
        static std::unordered_map<std::string, Font> s_fonts;
        static Shader * s_shader;
    };
}