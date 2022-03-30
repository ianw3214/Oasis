#include "application.hpp"
using namespace Oasis;

#include <chrono>

#include <imgui/imgui.h>

#include "util/trap.hpp"

#include "imgui/imguiWrapper.hpp"
#include "imgui/imgui_impl_opengl3.h"

#include "core/windowService.hpp"
#include "core/sceneManager.hpp"
#include "core/console.hpp"
#include "core/scene.hpp"
#include "systems/systemManager.hpp"

#include "resource/resourceManager.hpp"

#include "graphics/renderer.hpp"
#include "graphics/textrenderer.hpp"

#include "events/inputManager.hpp"
#include "events/event.hpp"

#include "audio/audio.hpp"

#include "entity/components/component.hpp"

#include "ui/uiManager.hpp"

#include <ryml.hpp>

Application::Application(const Configuration& config)
    : mWidth(config.mWidth)
    , mHeight(config.mHeight)
{
    mWindow = SDL_CreateWindow(
        config.mName,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.mWidth,
        config.mHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    OASIS_TRAP(mWindow);
    mContext = SDL_GL_CreateContext(mWindow);
    OASIS_TRAP(mContext);

    // Enable vsync
	SDL_GL_SetSwapInterval(1);

	// Setup blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GLEW
	GLenum glewError = glewInit();
	OASIS_TRAP(glewError == GLEW_OK);

    // Initialize subsystems
    WindowService::Init(this);
    ImGuiWrapper::Init();

    // Gameplay systems
    ComponentManager::InitCoreComponents();

    ResourceManager::Init();
    SceneManager::Init("res/config.yaml");
    SystemManager::Init();
    Renderer::Init();
    TextRenderer::Init();
    InputManager::Init(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    AudioEngine::Init();
    AudioEngine::SetListenerData();

    Console::Init();

    UIManager::Init();

    // Console commands
    Console::SetCommand("exit", [&](){
        mRunning = false;
    });

    // Initialize the starting scene
    SceneManager::CurrentScene()->Init();
}

Application::~Application()
{
    UIManager::Shutdown();
    
    SDL_DestroyWindow(mWindow);
}

void Application::OnEvent(const Event& event)
{
    if (event.GetType() == Oasis::EventType::WINDOW_CLOSE)
    {
        mRunning = false;
    }
    if (UIManager::HandleEvent(event))
    {
        return;
    }
    SceneManager::CurrentScene()->OnEvent(event);
}

void DisplayApplicationInfo(double * microseconds)
{
    double ms = static_cast<double>(*microseconds) * 1000.0;
    int fps = static_cast<int>(1000000.0 / *microseconds);

    static bool show = true;
    ImGui::Begin("APPLICATION INFO", &show, ImGuiWindowFlags_MenuBar);
    ImGui::Text("%d FPS (%f ms)", fps, ms);
    ImGui::End();
}

void Application::Run()
{
    // Initialize the duration to 24 fps
    mDelta = 1000000.0 / 24.0;
    ImGuiWrapper::AddWindowFunction(std::bind(DisplayApplicationInfo, &mDelta));

    mRunning = true;
    while(mRunning)
    {
        auto updateStart = std::chrono::system_clock::now();

        ////////////////////////////////////////////////////////////////
        Renderer::Clear({1.f, 0.f, 1.f});
        InputManager::Update();
        SceneManager::CurrentScene()->Update();
        SystemManager::Update(SceneManager::CurrentScene());
        UIManager::Update();

        ImGuiWrapper::Update(static_cast<float>(mDelta / 1000.0));

        SDL_GL_SwapWindow(mWindow);
        ////////////////////////////////////////////////////////////////

        mDelta = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - updateStart).count());
    }
}