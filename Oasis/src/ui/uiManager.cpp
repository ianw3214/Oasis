#include "uiManager.hpp"

#include <functional>

#include "util/trap.hpp"

#include "core/windowService.hpp"
#include "graphics/renderer.hpp"

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

    // DEBUG CODE
    UIElement * test = new UIElement();
    test->m_width = 100;
    test->m_height = 100;
    test->m_anchor = UIAnchor::CENTER;
    test->m_xOffset = -50;
    test->m_yOffset = -50;
    test->m_UIType = UIType::BACKGROUND;
    test->m_borderWidth = 3;
    test->m_background = Oasis::Colours::GREEN;
    test->m_border = Oasis::Colours::RED;
    s_root.m_children.push_back(test);
}

void UIManager::Update()
{
    typedef std::function<void(Ref<UIElement>, int, int, int, int)> f;
    f update_ui = [&](Ref<UIElement> curr, int parent_x, int parent_y, int parent_w, int parent_h) {
        OASIS_TRAP(parent_w >= 0 && parent_h >= 0);
        const int winWidth = Oasis::WindowService::WindowWidth();
        const int winHeight = Oasis::WindowService::WindowHeight();
        const unsigned int w = curr->m_width;
                const unsigned int h = curr->m_height;
        // Calculate the x/y of our current UI Element
        int x = 0;
        int y = 0;
        switch(curr->m_anchor)
        {
            case UIAnchor::TOP_LEFT: {
                x = 0, y = winHeight;
            } break;
            case UIAnchor::TOP_RIGHT: {
                x = winWidth, y = winHeight;
            } break;
            case UIAnchor::BOTTOM_LEFT: {
                x = 0 , y = 0;
            } break;
            case UIAnchor::BOTTOM_RIGHT: {
                x = winWidth, y = 0;
            } break;
            case UIAnchor::CENTER: {
                x = winWidth / 2;
                y = winHeight / 2;
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
            case UIType::BACKGROUND: {
                // Draw the background
                // TODO: Customizable colours
                Oasis::Renderer::DrawQuad((float) x, (float) y, (float) curr->m_width, (float) curr->m_height, curr->m_background);
                // Draw the border as a line strip
                float * border = new float[curr->m_borderWidth * 5 * 2];
                for (unsigned int i = 0; i < curr->m_borderWidth; ++i)
                {
                    unsigned int start = i * 5 * 2;
                    border[start++] = x + static_cast<float>(i);
                    border[start++] = y + static_cast<float>(i);
                    border[start++] = x + w - static_cast<float>(i);
                    border[start++] = y + static_cast<float>(i);
                    border[start++] = x + w - static_cast<float>(i);
                    border[start++] = y + h - static_cast<float>(i);
                    border[start++] = x + static_cast<float>(i);
                    border[start++] = y + h - static_cast<float>(i);
                    border[start++] = x + static_cast<float>(i);
                    border[start++] = y + static_cast<float>(i);
                }
                Oasis::Renderer::DrawLineStrip(border, curr->m_borderWidth * 5, curr->m_border);
            } break;
            default: {
                OASIS_TRAP(false && "UI Type should be assigned(Can be set to NONE)");
            } break;
        }
        // Recurse over children
        for (auto child : curr->m_children)
        {
            update_ui(child, x, y, w, h);
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