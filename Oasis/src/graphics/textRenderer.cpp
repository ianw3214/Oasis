#include "textrenderer.hpp"
using namespace Oasis;

#include "util/trap.hpp"

#include "core/windowService.hpp"

FT_Library TextRenderer::s_ft;
std::unordered_map<std::string, Font> TextRenderer::s_fonts;
Shader * TextRenderer::s_shader;

void TextRenderer::Init()
{
    FT_Error result = FT_Init_FreeType(&s_ft);
    OASIS_TRAP(result == FT_Err_Ok);
    s_shader = new Shader(kTextVertexPath, kTextFragmentPath);
    s_shader->setUniform1f("u_screenWidth", static_cast<float>(WindowService::WindowWidth()));
    s_shader->setUniform1f("u_screenHeight", static_cast<float>(WindowService::WindowHeight()));
}

void TextRenderer::Shutdown()
{
    delete s_shader;
    FT_Done_FreeType(s_ft);
}

void TextRenderer::LoadFont(const std::string& name, const std::string& path, int fontSize)
{
    FT_Face face;
    FT_Error result = FT_New_Face(s_ft, path.c_str(), 0, &face);
    OASIS_TRAP(result == FT_Err_Ok);

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    if (s_fonts.find(name) == s_fonts.end())
    {
        s_fonts[name] = Font();
    }
    else
    {
        // TODO: Probably need to free existing font
    }

    // Disable byte alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; ++c)
    {
        FT_Error result = FT_Load_Char(face, c, FT_LOAD_RENDER);
        OASIS_TRAP(result == FT_Err_Ok);

        int width = face->glyph->bitmap.width;
        int height = face->glyph->bitmap.rows;
        Texture * texture = new Texture(width, height);
        texture->SetData(face->glyph->bitmap.buffer, width * height, GL_RED);

        Character character = {
            texture,
            width,
            height,
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<GLuint>(face->glyph->advance.x)
        };
        s_fonts[name].m_map.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
}

const Font& TextRenderer::GetFont(const std::string& name)
{
    auto font = s_fonts.find(name);
    OASIS_TRAP(s_fonts.find(name) != s_fonts.end())
    return font->second;
}

void TextRenderer::DrawCharacter(const std::string& font, GLchar character, float x, float y, const Colour& colour)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    OASIS_TRAP(s_fonts.find(font) != s_fonts.end())
    Character ch = s_fonts[font].m_map[character];

    GLfloat xpos = x + ch.m_bearingX;
    // TODO: Want to be able to specify text alignment
    GLfloat ypos = y - ch.m_bearingY /*- (ch.m_height - ch.m_bearingY) */;

    GLfloat w = static_cast<float>(ch.m_width);
    GLfloat h = static_cast<float>(ch.m_height);
    // Update VBO for each character
    GLfloat vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 1.0 },            
        { xpos,     ypos,       0.0, 0.0 },
        { xpos + w, ypos,       1.0, 0.0 },

        { xpos,     ypos + h,   0.0, 1.0 },
        { xpos + w, ypos,       1.0, 0.0 },
        { xpos + w, ypos + h,   1.0, 1.0 }           
    };

    VertexArray vao;
    VertexBuffer vbo(vertices, sizeof(GLfloat) * 6 * 4);
    VertexBufferLayout layout;
    layout.pushFloat(4);
    vao.addBuffer(vbo, layout);

    ch.m_texture->bind();
    s_shader->bind();
    s_shader->setUniform3f("textColour", colour.r, colour.g, colour.b);
    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

int TextRenderer::DrawString(const std::string& font, const std::string& str, float x, float y, const Colour& colour)
{
    int length = 0;
    for (char c : str)
    {
        OASIS_TRAP(s_fonts.find(font) != s_fonts.end())
        // TODO: Optimization - can just pass in the found character instead of having to search again in DrawCharacter
        DrawCharacter(font, c, x, y, colour);
		// bit shift by 6 to get value in pixels
        Character ch = s_fonts[font].m_map[c];
        x += ch.m_advance >> 6;
        length += ch.m_advance >> 6;
    }
    return length;
}