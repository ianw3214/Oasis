#include "sandbox.hpp"

#include "oasis.h"
#include "graphics/opengl/texture.hpp"

void SandboxLayer::Init() 
{
    Ref<Oasis::AudioResource> audio = Oasis::ResourceManager::LoadResource<Oasis::AudioResource>("res/test.wav");
    Oasis::AudioSource * source = new Oasis::AudioSource();
    // source->Play(audio);

    Oasis::TextRenderer::LoadFont("testfont", "res/Munro.ttf");
    Oasis::TextRenderer::LoadFont("testfont2", "res/Munro.ttf", 64);

    Oasis::Console::Error("TEST ERROR");
}

void SandboxLayer::Close() 
{

}

bool SandboxLayer::HandleEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::MOUSE_PRESS)
    {
        UIManager::ToggleWindow("TEST UI ELEMENT");
    }
    return false;
}

void SandboxLayer::Update()
{
    Ref<Oasis::Texture> test = Oasis::ResourceManager::GetResource<Oasis::Texture>("res/animate.png");

    float linestrip[6] = {300.f, 0.f, 100.f, 100.f, 100.f, 200.f};

    Oasis::Renderer::DrawLine(0.f, 0.f, 1280.f, 720.f, Oasis::Colour{0.f, 1.f, 1.f});
    Oasis::Renderer::DrawLineStrip(linestrip, 3, Oasis::Colours::GREEN);
    Oasis::Renderer::DrawQuad(0.f, 0.f, 100.f, 100.f, Oasis::Colour{0.2f, 0.2f, 0.5f});
    Oasis::Renderer::DrawQuad(100.f, 100.f, 100.f, 100.f, test);

    Oasis::TextRenderer::DrawString("testfont2", "TEST ORIGIN testgg", 0.f, 0.f, Oasis::Colours::WHITE);
    Oasis::TextRenderer::DrawCharacter("testfont", 'c', 50.f, 50.f, Oasis::Colours::BLUE);
    Oasis::TextRenderer::DrawString("testfont", "test string", 50.f, 100.f, Oasis::Colours::WHITE);
    int length = Oasis::TextRenderer::DrawString("testfont2", "test string 2", 350.f, 200.f, Oasis::Colours::WHITE);
    Oasis::Renderer::DrawLine(350.f, Oasis::WindowService::WindowHeight() - 200.f, 350.f + length, Oasis::WindowService::WindowHeight() - 200.f, Oasis::Colours::RED);
    Oasis::Renderer::DrawQuad(350.f - 64.f, Oasis::WindowService::WindowHeight() - 200.f, 64.f, 64.f, Oasis::Colours::RED);

    Oasis::Sprite sprite("res/animate.png");
    sprite.SetPos(200.f, 200.f);
    sprite.SetDimensions(50.f, 50.f);
    sprite.SetSourcePos(30.f, 30.f);
    sprite.SetSourceDimensions(60.f, 60.f);
    Oasis::Renderer::DrawSprite(&sprite);

    static int i = 0;
    i++;
    UIManager::SetBoundVariableInt("TEST", i);

    Oasis::Renderer::DrawQuad(0.f, 0.f, 500.f, 500.f, Oasis::Colours::WHITE, 0.2f);

    static bool initialized = false;
    static Oasis::AnimatedSprite sprite2("res/animate.png", 64.f, 80.f);
    if (!initialized)
    {
        initialized = true;
        sprite2.SetPos(300.f, 300.f);
        sprite2.SetDimensions(200.f, 200.f);
        sprite2.SetTint(Oasis::Colours::RED, 1.f);
        sprite2.AddAnimation("default", 0, 1);
        sprite2.PlayAnimation("default");
    }
    Oasis::Renderer::DrawAnimatedSprite(&sprite2);

}