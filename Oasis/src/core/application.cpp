#include "application.hpp"
using namespace Oasis;

#include <chrono>

#include <imgui/imgui.h>

#include "util/trap.hpp"

#include "imgui/imguiWrapper.hpp"
#include "imgui/imgui_impl_opengl3.h"

#include "core/windowService.hpp"
#include "core/state.hpp"
#include "core/stateManager.hpp"
#include "core/console.hpp"

#include "resource/resourceManager.hpp"

#include "graphics/renderer.hpp"
#include "graphics/textrenderer.hpp"

#include "events/inputManager.hpp"
#include "events/event.hpp"

#include "audio/audio.hpp"

Application::Application(const Configuration& config)
    : m_width(config.m_width)
    , m_height(config.m_height)
{
    m_window = SDL_CreateWindow(
        config.m_name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.m_width,
        config.m_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    OASIS_TRAP(m_window);
    m_context = SDL_GL_CreateContext(m_window);
    OASIS_TRAP(m_context);

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

    ResourceManager::Init();
    StateManager::Init(config.m_initState());
    Renderer::Init();
    TextRenderer::Init();
    InputManager::Init(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    AudioEngine::Init();
    AudioEngine::SetListenerData();

    Console::Init();

    // Console commands
    Console::SetCommand("exit", [&](){
        m_running = false;
    });
}

Application::~Application()
{
    SDL_DestroyWindow(m_window);
}

void Application::OnEvent(const Event& event)
{
    if (event.GetType() == Oasis::EventType::WINDOW_CLOSE)
    {
        m_running = false;
    }
    StateManager::CurrentState()->OnEvent(event);
}

void DisplayApplicationInfo(double * microseconds)
{
    double ms = static_cast<double>(*microseconds) / 1000.0;
    int fps = static_cast<int>(1000000.0 / *microseconds);

    static bool show = true;
    ImGui::Begin("APPLICATION INFO", &show, ImGuiWindowFlags_MenuBar);
    ImGui::Text("%d FPS (%f ms)", fps, ms);
    ImGui::End();
}

void Application::Run()
{
    // Initialize the duration to 24 fps
    double duration = 1000000.0 / 24.0;
    ImGuiWrapper::AddWindowFunction(std::bind(DisplayApplicationInfo, &duration));

    // TODO: Move this somewhere else
    StateManager::CurrentState()->Init();
    m_running = true;
    while(m_running)
    {
        auto updateStart = std::chrono::system_clock::now();

        ////////////////////////////////////////////////////////////////
        Renderer::Clear({1.f, 0.f, 1.f});
        InputManager::Update();
        StateManager::CurrentState()->Update();

        ImGuiWrapper::Update(static_cast<float>(duration / 1000.0));

        SDL_GL_SwapWindow(m_window);
        ////////////////////////////////////////////////////////////////

        duration = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - updateStart).count());
    }
}