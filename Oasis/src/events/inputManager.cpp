#include "inputManager.hpp"
using namespace Oasis;

#include <SDL2/SDL.h>

#include "core/application.hpp"
#include "core/console.hpp"

#include <imgui/imgui.h>
#include "imgui/imguiWrapper.hpp"

#include "events/windowEvent.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvent.hpp"
#include "events/textEvent.hpp"

#include "core/windowService.hpp"

std::function<void(Event&)> InputManager::s_eventCallback;
bool InputManager::s_mouseDown;

void InputManager::Init(std::function<void(Event&)> callback)
{
    s_eventCallback = callback;
    s_mouseDown = false;
}

void InputManager::Update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGuiWrapper::OnEvent(e);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        
        if (e.type == SDL_QUIT)
        {
            WindowCloseEvent WindowCloseEvent;
            s_eventCallback(WindowCloseEvent);
        }

        if (e.type == SDL_KEYDOWN && !io.WantCaptureKeyboard)
        {
            KeyPressedEvent keyEvent(e.key.keysym.scancode);
            s_eventCallback(keyEvent);

            // Extra buttons to handle engine stuff
            if (e.key.keysym.scancode == SDL_SCANCODE_GRAVE)
            {
                Oasis::Console::ToggleShow();
            }
        }

        if (e.type == SDL_KEYUP && !io.WantCaptureKeyboard)
        {
            KeyReleasedEvent keyEvent(e.key.keysym.scancode);
            s_eventCallback(keyEvent);
        }

        // TODO: SPECIFY BETWEEN DIFFERENT MOUSE BUTTONS
        if (e.type == SDL_MOUSEBUTTONDOWN && !io.WantCaptureMouse)
        {
            MousePressedEvent mouseEvent(e.button.x, Oasis::WindowService::WindowHeight() - e.button.y);
            s_eventCallback(mouseEvent);
            s_mouseDown = true;
        }

        if (e.type == SDL_MOUSEBUTTONUP && !io.WantCaptureMouse)
        {
            MouseReleasedEvent mouseEvent(e.button.x, Oasis::WindowService::WindowHeight() - e.button.y);
            s_eventCallback(mouseEvent);
            s_mouseDown = false;
        }

        if (e.type == SDL_MOUSEMOTION && !io.WantCaptureMouse)
        {
            MouseMovedEvent mouseEvent(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
            s_eventCallback(mouseEvent);
        }

        if (e.type == SDL_MOUSEWHEEL && !io.WantCaptureMouse)
        {
            MouseScrolledEvent mouseEvent(e.wheel.x, e.wheel.y);
            s_eventCallback(mouseEvent);
        }

        if (e.type == SDL_TEXTINPUT && !io.WantCaptureKeyboard)
        {
            TextInputEvent textEvent(e.text.text);
            s_eventCallback(textEvent);
        }
    }
}