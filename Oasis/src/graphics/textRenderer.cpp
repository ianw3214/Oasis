#include "textrenderer.hpp"
using namespace Oasis;

FT_Library TextRenderer::s_ft;
std::unordered_map<GLchar, Character> TextRenderer::s_characters;
Shader * TextRenderer::s_shader;

void TextRenderer::Init()
{
    if (FT_Init_FreeType(&s_ft))
    {
        // TODO: ERROR HANDLING
    }
    s_shader = new Shader(kTextVertexPath, kTextFragmentPath);
}

void TextRenderer::Shutdown()
{
    delete s_shader;
    FT_Done_FreeType(s_ft);
}

void TextRenderer::LoadFont(const std::string& path, int fontSize)
{
    FT_Face face;
    if (FT_New_Face(s_ft, path.c_str(), 0, &face))
    {
        // TODO: ERROR HANDLING
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; ++c)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            // TODO: ERROR HANDLING
        }
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
        s_characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
}

// TODO: Don't use glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

void TextRenderer::DrawCharacter(GLchar character, float x, float y)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f);

    Character ch = s_characters[character];

    GLfloat xpos = x + ch.m_bearingX;
    GLfloat ypos = y - (ch.m_height - ch.m_bearingY);

    GLfloat w = static_cast<float>(ch.m_width);
    GLfloat h = static_cast<float>(ch.m_height);
    // Update VBO for each character
    GLfloat vertices[6][4] = {
        { xpos,     ypos + h,   0.0, 0.0 },            
        { xpos,     ypos,       0.0, 1.0 },
        { xpos + w, ypos,       1.0, 1.0 },

        { xpos,     ypos + h,   0.0, 0.0 },
        { xpos + w, ypos,       1.0, 1.0 },
        { xpos + w, ypos + h,   1.0, 0.0 }           
    };

    VertexArray vao;
    VertexBuffer vbo(vertices, sizeof(GLfloat) * 6 * 4);
    VertexBufferLayout layout;
    layout.pushFloat(4);
    vao.addBuffer(vbo, layout);

    ch.m_texture->bind();
    s_shader->bind();
    s_shader->setUniform3f("textColour", 1.f, 1.f, 1.f);
    s_shader->setUniformMat4("projection", glm::value_ptr(projection));
    vao.bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TextRenderer::DrawString(const std::string& str, float x, float y)
{
    for (char c : str)
    {
        Character ch = s_characters[c];
        DrawCharacter(c, x, y);
		// bit shift by 6 to get value in pixels
        x += ch.m_advance >> 6;
    }
}