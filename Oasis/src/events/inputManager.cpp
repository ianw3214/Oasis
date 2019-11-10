#include "inputManager.hpp"
using namespace Oasis;

#include <SDL2/SDL.h>

#include "core/application.hpp"

#include "events/windowEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvent.hpp"

std::function<void(Event&)> InputManager::s_eventCallback;

void InputManager::Init(std::function<void(Event&)> callback)
{
    s_eventCallback = callback;
}

void InputManager::Update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            WindowCloseEvent WindowCloseEvent;
            s_eventCallback(WindowCloseEvent);
        }

        if (e.type == SDL_KEYDOWN)
        {
            KeyPressedEvent keyEvent(e.key.keysym.scancode);
            s_eventCallback(keyEvent);
        }

        if (e.type == SDL_KEYUP)
        {
            KeyReleasedEvent keyEvent(e.key.keysym.scancode);
            s_eventCallback(keyEvent);
        }

        // TODO: SPECIFY BETWEEN DIFFERENT MOUSE BUTTONS
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            MousePressedEvent mouseEvent(e.button.x, e.button.y);
            s_eventCallback(mouseEvent);
        }

        if (e.type == SDL_MOUSEMOTION)
        {
            MouseMovedEvent mouseEvent(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
            s_eventCallback(mouseEvent);
        }
    }
}