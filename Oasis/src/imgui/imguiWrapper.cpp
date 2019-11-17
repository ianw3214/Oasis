#include "imguiWrapper.hpp"
using namespace Oasis;

#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>

#include "core/windowService.hpp"

#include "events/inputManager.hpp"
#include "events/event.hpp"
#include "events/keyEvent.hpp"
#include "events/mouseEvent.hpp"
#include "events/textEvent.hpp"

std::vector<std::function<void()>> ImGuiWrapper::s_windowFunctions;

// Data
static SDL_Cursor * g_mouseCursors[ImGuiMouseCursor_COUNT] = {};

void ImGuiWrapper::Init()
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(static_cast<float>(WindowService::WindowWidth()), static_cast<float>(WindowService::WindowHeight()));
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // TODO: Use own key codes
    io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = SDL_SCANCODE_RETURN2;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

    g_mouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    g_mouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    g_mouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    g_mouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    g_mouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    g_mouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    g_mouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    g_mouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiWrapper::Shutdown()
{
    
}

void ImGuiWrapper::AddWindowFunction(std::function<void()> func)
{
    s_windowFunctions.push_back(func);
}

void ImGuiWrapper::OnEvent(const Event & event)
{
    ImGuiIO& io = ImGui::GetIO();
    if (event.GetType() == EventType::MOUSE_MOVE)
    {
        const MouseMovedEvent& mouseEvent = dynamic_cast<const MouseMovedEvent&>(event);
        io.MousePos = ImVec2(static_cast<float>(mouseEvent.GetX()), static_cast<float>(mouseEvent.GetY()));
    }
    if (event.GetType() == EventType::MOUSE_SCROLL)
    {
        const MouseScrolledEvent& mouseEvent = dynamic_cast<const MouseScrolledEvent&>(event);
        if (mouseEvent.GetHorizontalScroll() > 0) io.MouseWheelH += 1;
        if (mouseEvent.GetHorizontalScroll() < 0) io.MouseWheelH -= 1;
        if (mouseEvent.GetVerticalScroll() > 0) io.MouseWheel += 1;
        if (mouseEvent.GetVerticalScroll() < 0) io.MouseWheel -= 1;
    }
    if (event.GetType() == EventType::TEXT_INPUT)
    {
        const TextInputEvent& textEvent = dynamic_cast<const TextInputEvent&>(event);
        io.AddInputCharactersUTF8(textEvent.GetText());
    }
    if (event.GetType() == EventType::KEY_PRESSED)
    {
        const KeyPressedEvent& keyEvent = dynamic_cast<const KeyPressedEvent&>(event);
        io.KeysDown[keyEvent.GetKey()] = true;
        // TODO: Find a better way to handle this
        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
        io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
    }
    if (event.GetType() == EventType::KEY_RELEASED)
    {
        const KeyReleasedEvent& keyEvent = dynamic_cast<const KeyReleasedEvent&>(event);
        io.KeysDown[keyEvent.GetKey()] = false;
        // TODO: Find a better way to handle this
        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
        io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
    }
}

static void UpdateInputStates()
{
    ImGuiIO& io = ImGui::GetIO();

    io.MouseDown[0] = InputManager::MouseHeld();

    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0)
    {
        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            SDL_ShowCursor(SDL_FALSE);
        }
        else
        {
            // Show OS mouse cursor
            SDL_SetCursor(g_mouseCursors[imgui_cursor] ? g_mouseCursors[imgui_cursor] : g_mouseCursors[ImGuiMouseCursor_Arrow]);
            SDL_ShowCursor(SDL_TRUE);
        }
    }
}

void ImGuiWrapper::Update(float deltaTime)
{
    UpdateInputStates();

    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = deltaTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    for (auto func : s_windowFunctions)
    {
        func();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
