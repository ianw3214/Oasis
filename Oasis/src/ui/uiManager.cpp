#include "uiManager.hpp"

#include <functional>

#include "util/trap.hpp"

#include "core/windowService.hpp"
#include "graphics/renderer.hpp"
#include "graphics/textRenderer.hpp"
#include "graphics/sprite.hpp"

// The root is the entire window
UIElement UIManager::s_root;
std::unordered_map<std::string, Ref<UIElement>> UIManager::s_UIElements;

void UIManager::Init()
{
    // Initialize the font resources used for UI
    Oasis::TextRenderer::LoadFont(GetUIFont(UIFont::DEFAULT), GetUIFontPath(UIFont::DEFAULT), GetUIFontSize(UIFont::DEFAULT));
    Oasis::TextRenderer::LoadFont(GetUIFont(UIFont::SMALL), GetUIFontPath(UIFont::SMALL), GetUIFontSize(UIFont::SMALL));

    // Only some of these matter for the calculation but we set all of them anyways
    s_root.m_anchor = UIAnchor::TOP_LEFT;
    s_root.m_width = Oasis::WindowService::WindowWidth();
    s_root.m_height = Oasis::WindowService::WindowHeight();
    s_root.m_xOffset = 0;
    s_root.m_yOffset = 0;
    s_root.m_UIType = UIType::NONE;

    ////////////////////////////////////////////////////////////////
    // DEBUG CODE
    ////////////////////////////////////////////////////////////////
    UIElement * test = new UIElement();
    test->m_width = 100;
    test->m_height = 100;
    test->m_anchor = UIAnchor::CENTER;
    test->m_xOffset = -50;
    test->m_yOffset = -50;
    test->m_UIType = UIType::BACKGROUND;
    test->m_borderWidth = 3;
    test->m_background = Oasis::Colours::BLACK;
    test->m_border = Oasis::Colours::WHITE;
    s_root.m_children.push_back(test);

    UIElement * text = new UIElement();
    // text->m_width = 100;
    // text->m_height = 100;
    text->m_anchor = UIAnchor::TOP_LEFT;
    text->m_xOffset = 0;
    text->m_yOffset = 0;
    text->m_UIType = UIType::TEXT;
    text->m_text = "HELLO WORLD";
    text->m_colour = Oasis::Colours::GREEN;
    text->m_font = UIFont::DEFAULT;
    test->m_children.push_back(text);

    UIElement * text2 = new UIElement();
    // text->m_width = 100;
    // text->m_height = 100;
    text2->m_anchor = UIAnchor::BOTTOM_RIGHT;
    text2->m_xOffset = 0;
    text2->m_yOffset = 0;
    text2->m_UIType = UIType::TEXT;
    text2->m_text = "HELLO WORLD 2";
    text2->m_colour = Oasis::Colours::GREEN;
    text2->m_font = UIFont::SMALL;
    test->m_children.push_back(text2);

    UIElement * img = new UIElement();
    img->m_width = 30;
    img->m_height = 30;
    img->m_anchor = UIAnchor::CENTER;
    img->m_xOffset = 0;
    img->m_yOffset = 0;
    img->m_UIType = UIType::TEXTURE;
    img->m_path = "res/animate.png";
    test->m_children.push_back(img);
    ////////////////////////////////////////////////////////////////
}

void UIManager::Update()
{
    typedef std::function<void(Ref<UIElement>, int, int, int, int)> f;
    f update_ui = [&](Ref<UIElement> curr, int parent_x, int parent_y, int parent_w, int parent_h) {
        OASIS_TRAP(parent_w >= 0 && parent_h >= 0);
        const unsigned int w = curr->m_width;
                const unsigned int h = curr->m_height;
        // Calculate the x/y of our current UI Element
        int x = 0;
        int y = 0;
        switch(curr->m_anchor)
        {
            case UIAnchor::TOP_LEFT: {
                x = parent_x, y = parent_y;
            } break;
            case UIAnchor::TOP_RIGHT: {
                x = parent_x + parent_w, y = parent_y;
            } break;
            case UIAnchor::BOTTOM_LEFT: {
                x = parent_x, y = parent_y + parent_h;
            } break;
            case UIAnchor::BOTTOM_RIGHT: {
                x = parent_x + parent_w, y = parent_y + parent_h;
            } break;
            case UIAnchor::CENTER: {
                x = parent_x + parent_w / 2;
                y = parent_y + parent_h / 2;
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
            case UIType::TEXT: {
                // TODO: Fix these issues in engine (or maybe not)
                // Text drawing as also actually top aligned
                const float y_adjusted = static_cast<float>(y + GetUIFontSize(curr->m_font));
                const int length = Oasis::TextRenderer::DrawString(GetUIFont(curr->m_font), std::string(curr->m_text), (float) x, y_adjusted, curr->m_colour);
            } break;
            case UIType::TEXTURE: {
                // Cache the sprite so we don't have to constantly recreate it
                if (!curr->m_cachedSprite)
                {
                    curr->m_cachedSprite = new Oasis::Sprite(curr->m_path);
                }
                curr->m_cachedSprite->SetDimensions((float)curr->m_width, (float)curr->m_height);
                curr->m_cachedSprite->SetPos((float)x, (float)y);
                Oasis::Renderer::DrawSprite(curr->m_cachedSprite);
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