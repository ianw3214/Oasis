#include "renderer.hpp"
using namespace Oasis;

#include <GL/glew.h>

#include "opengl/glwrappers.hpp"

// Index orders for different shapes
const unsigned int LINE_INDICES[2] = { 0, 1 };
const unsigned int SQUARE_INDICES[6] = { 0, 1, 3, 0, 2, 3 };

Shader * basicShader;
Shader * textureShader;

void Renderer::Init()
{
    basicShader = new Shader("res/shaders/basic_vertex.glsl", "res/shaders/basic_fragment.glsl");
	textureShader = new Shader("res/shaders/basic_vertex.glsl", "res/shaders/basic_texture.glsl");
}

void Renderer::Clear(Colour colour)
{
    glClearColor(colour.r, colour.g, colour.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// TODO: Move this somewhere else
template<typename T>
T lerp(const T& start, const T& end, float percentage) {
	return start + static_cast<T>(static_cast<float>(end - start) * percentage);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, const Colour& colour)
{
    float positions[4] = { x1, y1, x2, y2 };
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 4);
	IndexBuffer		ib(LINE_INDICES, 2);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Set the uniform to draw the right colour
	basicShader->setUniform4f("u_Colour", colour.r, colour.g, colour.b, 1.f);
	basicShader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawQuad(float x, float y, float w, float h, const Colour& colour)
{
    // TODO: Store buffers somewhere and adjust unfiforms to draw
    float positions[8] = {
        x, y,
        x, y + h,
        x + w, y,
        x + w, y + h
    };
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 8);
	IndexBuffer		ib(SQUARE_INDICES, 6);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Issue the actual draw call
    basicShader->setUniform4f("u_Colour", colour.r, colour.g, colour.b, 1.f);
	basicShader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawQuad(float x, float y, float w, float h, const Texture& texture)
{
	float positions[16] = {
		x, y, 0.f, 0.f,
		x, y + h, 0.f, 1.f,
		x + w, y, 1.f, 0.f,
		x + w, y + h, 1.f, 1.f
	};
	VertexArray		va;
	VertexBuffer	vb(positions, sizeof(float) * 16);
	IndexBuffer		ib(SQUARE_INDICES, 6);
	// Specify the layout of the buffer data
	VertexBufferLayout layout;
	layout.pushFloat(2);
	layout.pushFloat(2);
	va.addBuffer(vb, layout);

	// Bind the texture and draw
	texture.bind();
	textureShader->bind();
	va.bind();
	ib.bind();

	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}