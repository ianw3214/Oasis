#include "application.hpp"
using namespace Oasis;

#include <chrono>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>

#include "util/trap.hpp"

#include "core/windowService.hpp"
#include "core/state.hpp"
#include "core/stateManager.hpp"

#include "resource/resourceManager.hpp"

#include "graphics/renderer.hpp"
#include "graphics/textrenderer.hpp"

#include "events/inputManager.hpp"
#include "events/event.hpp"

#include "imgui/imguiWrapper.hpp"
#include "imgui/imgui_impl_opengl3.h"

#include "audio/audio.hpp"

struct Application::Impl
{
    SDL_Window * m_window;
    SDL_GLContext m_context;
};

Application::Application(const Configuration& config)
    : m_width(config.m_width)
    , m_height(config.m_height)
    , m_impl(new Impl())
{
    m_impl->m_window = SDL_CreateWindow(
        config.m_name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config.m_width,
        config.m_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    OASIS_TRAP(m_impl->m_window);
    m_impl->m_context = SDL_GL_CreateContext(m_impl->m_window);
    OASIS_TRAP(m_impl->m_context);

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
    ResourceManager::Init();
    StateManager::Init(config.m_initState());
    Renderer::Init();
    TextRenderer::Init();
    InputManager::Init(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    ImGuiWrapper::Init();
    AudioEngine::Init();
    AudioEngine::SetListenerData();
}

Application::~Application()
{
    SDL_DestroyWindow(m_impl->m_window);
    delete m_impl;
}

void Application::OnEvent(const Event& event)
{
    if (event.GetType() == Oasis::EventType::WINDOW_CLOSE)
    {
        m_running = false;
    }
    StateManager::CurrentState()->OnEvent(event);
    ImGuiWrapper::OnEvent(event);
}

// TODO: Make this a callback to push onto the ImGui wrapper
static void DisplayApplicationInfo(double microseconds)
{
    double ms = static_cast<double>(microseconds) / 1000.0;
    int fps = static_cast<int>(1000000.0 / microseconds);

    // TODO: Move this back to ImGui wrapper
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::Begin("APPLICATION INFO", &show, ImGuiWindowFlags_MenuBar);
    ImGui::Text("%d FPS (%f ms)", fps, ms);
    ImGui::End();
}

void Application::Run()
{
    // Initialize the duration to 24 fps
    double duration = 1000000.0 / 24.0;

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

        DisplayApplicationInfo(duration);
        ImGuiWrapper::Update(static_cast<float>(duration / 1000.0));

        SDL_GL_SwapWindow(m_impl->m_window);
        ////////////////////////////////////////////////////////////////

        duration = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - updateStart).count());
    }
}