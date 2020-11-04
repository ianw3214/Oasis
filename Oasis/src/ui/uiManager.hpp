#pragma once

#include "util/pointer.hpp"

#include <string>
#include <unordered_map>

#include "uiCommon.hpp"
#include "uiSerializer.hpp"
#include "uiBoundVariables.hpp"

// Engine subsystems should be static classes
class UIManager
{
public:
    static void Init();
    static void Shutdown();
    static void Update();

    static Ref<UIElement> GetUIElement(const std::string& name);

    static void ShowWindow(const std::string& name);
    static void HideWindow(const std::string& name);
    static void ToggleWindow(const std::string& name);

    static void SetBoundVariableInt(const std::string& name, int val);
    static void SetBoundVariableUInt(const std::string& name, unsigned int val);
    static void SetBoundVariableStr(const std::string& name, const std::string& val);
private:
    static UIElement s_root;
    // Allow the access of UI elements from other parts of the code
    static std::unordered_map<std::string, Ref<UIElement>> s_UIElements;

    // Useful subsystems
    static UISerializer* s_serializer;
    static UIBoundVariables * s_boundVariables;
private:
    // Helper functions
    static void DeserializeUI();
};