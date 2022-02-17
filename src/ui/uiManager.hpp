#pragma once

#include "util/pointer.hpp"
#include "events/event.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "uiCommon.hpp"
#include "uiSerializer.hpp"
#include "uiBoundVariables.hpp"
#include "uiEventManager.hpp"

// Engine subsystems should be static classes
class UIManager
{
public:
    static inline std::string Path() { return "res/ui"; }
public:
    static void Init();
    static void Shutdown();
    static void Update();
    static bool HandleEvent(const Oasis::Event& event);

    static Ref<UIElement> GetUIElement(const std::string& name);

    static void ShowWindow(const std::string& name);
    static void HideWindow(const std::string& name);
    static void ToggleWindow(const std::string& name);

    static void SetBoundVariableInt(const std::string& name, int val);
    static void SetBoundVariableUInt(const std::string& name, unsigned int val);
    static void SetBoundVariableStr(const std::string& name, const std::string& val);

    static void AddUIEventCallback(const std::string& event_key, std::function<void()> callback);
private:
    static UIElement s_root;
    // Allow the access of UI elements from other parts of the code
    static std::unordered_map<std::string, Ref<UIElement>> s_UIElements;

    // Useful subsystems
    static UISerializer* s_serializer;
    static UIBoundVariables * s_boundVariables;
    static UIEventManager * s_eventManager;
private:
    // Helper functions
    static void DeserializeUI();
};