#include "sandbox.hpp"

#include "oasis.h"
#include "graphics/opengl/texture.hpp"

void Sandbox::Init() 
{
    Texture test("res/animate.png");

    Oasis::Renderer::DrawLine(0.f, 0.f, 1.f, 1.f, Oasis::Colour{0.f, 1.f, 1.f});
    Oasis::Renderer::DrawQuad(-1.f, -1.f, 1.f, 1.f, Oasis::Colour{0.2f, 0.2f, 0.5f});
    Oasis::Renderer::DrawQuad(-1.f, 0.f, 1.f, 1.f, test);

    ALuint buffer = Oasis::AudioEngine::LoadSound("res/test.wav");
    Oasis::AudioSource * source = new Oasis::AudioSource();
    // source->Play(buffer);

    Oasis::TextRenderer::LoadFont("res/Munro.ttf");
}

void Sandbox::Close() 
{

}

void Sandbox::OnEvent(const Oasis::Event& event)
{
    
}

void Sandbox::Update()
{
    Texture test("res/animate.png");

    Oasis::Renderer::DrawLine(0.f, 0.f, 1.f, 1.f, Oasis::Colour{0.f, 1.f, 1.f});
    Oasis::Renderer::DrawQuad(-1.f, -1.f, 1.f, 1.f, Oasis::Colour{0.2f, 0.2f, 0.5f});
    Oasis::Renderer::DrawQuad(-1.f, 0.f, 1.f, 1.f, test);

    Oasis::TextRenderer::DrawCharacter('c', 50.f, 50.f);
}