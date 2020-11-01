#include "uiManager.hpp"

#include <functional>

#include "util/trap.hpp"

#include "core/windowService.hpp"

// The root is the entire window
UIElement UIManager::s_root;
std::unordered_map<std::string, Ref<UIElement>> UIManager::s_UIElements;

void UIManager::Init()
{
    // These don't actually matter for the root but we'll leave them in here anyways just in case
    s_root.m_anchor = UIAnchor::TOP_LEFT;
    s_root.m_width = Oasis::WindowService::WindowWidth();
    s_root.m_height = Oasis::WindowService::WindowHeight();
    s_root.m_xOffset = 0;
    s_root.m_yOffset = 0;
    // This is the only one that matters for root
    s_root.m_UIType = UIType::NONE;
}

void UIManager::Update()
{
    typedef std::function<void(Ref<UIElement>, int, int, int, int)> f;
    f update_ui = [&](Ref<UIElement> curr, int parent_x, int parent_y, int parent_w, int parent_h) {
        OASIS_TRAP(parent_w >= 0 && parent_h >= 0);
        const int winWidth = Oasis::WindowService::WindowWidth();
        const int winHeight = Oasis::WindowService::WindowHeight();
        // Calculate the x/y of our current UI Element
        int x = 0;
        int y = 0;
        switch(curr->m_anchor)
        {
            case UIAnchor::TOP_LEFT: {
                x = 0, y = 0;
            } break;
            case UIAnchor::TOP_RIGHT: {
                x = winWidth, y = 0;
            } break;
            case UIAnchor::BOTTOM_LEFT: {
                x = 0 , y = winHeight;
            } break;
            case UIAnchor::BOTTOM_RIGHT: {
                x = winWidth, y = winHeight;
            } break;
            case UIAnchor::CENTER: {
                x = winWidth / 2;
                y = winHeight;
            } break;
            default: {
                OASIS_TRAP(false && "UI Anchor should be assigned");
            } break;
        };
        x += curr->m_xOffset;
        y += curr->m_yOffset;
        switch(curr->m_UIType)
        {
            case UIType::NONE: {
                // Do nothing...
            } break;
        }
        // Recurse over children
        for (auto child : curr->m_children)
        {
            update_ui(child, x, y, curr->m_width, curr->m_height);
        }
        return;
    };
    update_ui(&s_root, 0, 0, Oasis::WindowService::WindowWidth(), Oasis::WindowService::WindowHeight());
}

Ref<UIElement> UIManager::GetUIElement(const std::string& name)
{
    auto it = s_UIElements.find(name);
    if (it != s_UIElements.end())
    {
        return it->second;
    }
    return nullptr;
}