#pragma once

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "opengl/glwrappers.hpp"

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
        // Offset to advance to the next glyph
        GLuint m_advance;
    };

    const std::string kTextVertexPath = "res/shaders/text_vertex.glsl";
    const std::string kTextFragmentPath = "res/shaders/text_fragment.glsl";

    //////////////////////////////////////////////////////////////////////////
    class TextRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void LoadFont(const std::string& path);

        static void DrawCharacter(GLchar character, float x, float y);
    private:
        static FT_Library s_ft;
        static std::unordered_map<GLchar, Character> s_characters;
        static Shader * s_shader;
    };
}