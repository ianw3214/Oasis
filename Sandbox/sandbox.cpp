#include "sandbox.hpp"

#include "oasis.h"
#include "graphics/opengl/texture.hpp"

void Sandbox::Init() 
{
    ALuint buffer = Oasis::AudioEngine::LoadSound("res/test.wav");
    Oasis::AudioSource * source = new Oasis::AudioSource();
    // source->Play(buffer);
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

    Oasis::Renderer::DrawLine(0.f, 0.f, 1.f, 1.f, {0.f, 1.f, 1.f});
    Oasis::Renderer::DrawQuad(-1.f, -1.f, 1.f, 1.f, {0.2f, 0.2f, 0.5f});
    Oasis::Renderer::DrawQuad(-1.f, 0.f, 1.f, 1.f, test);
}