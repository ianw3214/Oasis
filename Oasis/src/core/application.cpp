#include "application.hpp"
using namespace Oasis;

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "util/trap.hpp"

#include "core/windowService.hpp"
#include "core/state.hpp"
#include "core/stateManager.hpp"

#include "graphics/renderer.hpp"
#include "graphics/textrenderer.hpp"

#include "events/inputManager.hpp"
#include "events/event.hpp"

#include "imgui/imguiWrapper.hpp"

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
    StateManager::Init(config.m_state);
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

void Application::Run()
{
    // TODO: Move this somewhere else
    StateManager::CurrentState()->Init();
    m_running = true;
    while(m_running)
    {
        Renderer::Clear({1.f, 0.f, 1.f});
        InputManager::Update();
        StateManager::CurrentState()->Update();

        ImGuiWrapper::Update();

        SDL_GL_SwapWindow(m_impl->m_window);
    }
}