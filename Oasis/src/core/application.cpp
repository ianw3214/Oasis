#include "application.hpp"
using namespace Oasis;

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "core/state.hpp"
#include "core/stateManager.hpp"

#include "graphics/renderer.hpp"

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
    if (m_impl->m_window == nullptr)
    {
        // TODO: ERROR HANDLING
    }
    m_impl->m_context = SDL_GL_CreateContext(m_impl->m_window);
    if (m_impl->m_context == NULL)
    {
        // TODO: ERROR HANDLING
    }

    // Enable vsync
	SDL_GL_SetSwapInterval(1);

	// Setup blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		// TODO: ERROR HANDLING
	}

    // Initialize subsystems
    StateManager::Init(config.m_state);
    Renderer::Init();

}

Application::~Application()
{
    SDL_DestroyWindow(m_impl->m_window);
    delete m_impl;
}

void Application::Run()
{
    bool running = true;
    while(running)
    {
        Renderer::Clear({1.f, 0.f, 1.f});
        // TEMPORARY INPUT CODE
        // TODO: MOVE ELSEWHERE
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
        }
        StateManager::CurrentState()->Update();
        SDL_GL_SwapWindow(m_impl->m_window);
    }
}