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

#include "graphics/renderer.hpp"
#include "graphics/textRenderer.hpp"

std::function<void(Event&)> InputManager::s_eventCallback;
bool InputManager::s_mouseDown[static_cast<int>(MouseButton::COUNT)];

void InputManager::Init(std::function<void(Event&)> callback)
{
    s_eventCallback = callback;
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
            MouseButton button;
            switch(e.button.button) {
                case SDL_BUTTON_LEFT:
                    button = MouseButton::LEFT;
                    break;
                case SDL_BUTTON_RIGHT:
                    button = MouseButton::RIGHT;
                    break;
                default:
                    button = MouseButton::LEFT;
            }
            MousePressedEvent mouseEvent(e.button.x, Oasis::WindowService::WindowHeight() - e.button.y, button);
            s_eventCallback(mouseEvent);
            s_mouseDown[static_cast<int>(button)] = true;
        }

        if (e.type == SDL_MOUSEBUTTONUP && !io.WantCaptureMouse)
        {
            MouseButton button;
            switch(e.button.button) {
                case SDL_BUTTON_LEFT:
                    button = MouseButton::LEFT;
                    break;
                case SDL_BUTTON_RIGHT:
                    button = MouseButton::RIGHT;
                    break;
                default:
                    button = MouseButton::LEFT;
            }
            MouseReleasedEvent mouseEvent(e.button.x, Oasis::WindowService::WindowHeight() - e.button.y, button);
            s_eventCallback(mouseEvent);
            s_mouseDown[static_cast<int>(button)] = false;
        }

        if (e.type == SDL_MOUSEMOTION && !io.WantCaptureMouse)
        {
            MouseMovedEvent mouseEvent(e.motion.x, Oasis::WindowService::WindowHeight() - e.motion.y, e.motion.xrel, -e.motion.yrel);
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

        // TODO: Pass these on to the actual application as well
        if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                Oasis::Renderer::SetWindowSize(e.window.data1, e.window.data2);
                Oasis::WindowService::SetWindowDimensions(e.window.data1, e.window.data2);
                Oasis::TextRenderer::ResetWindowDimensions();
            }
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                Oasis::Renderer::SetWindowSize(e.window.data1, e.window.data2);
                Oasis::WindowService::SetWindowDimensions(e.window.data1, e.window.data2);
                Oasis::TextRenderer::ResetWindowDimensions();
            }
        }
    }
}

bool InputManager::MouseHeld(MouseButton button) {
    return s_mouseDown[static_cast<int>(button)];
}