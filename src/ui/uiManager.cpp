#include "uiManager.hpp"

#include <functional>
#include <filesystem>

#include "util/trap.hpp"

#include "core/windowService.hpp"
#include "graphics/renderer.hpp"
#include "graphics/textRenderer.hpp"
#include "graphics/sprite.hpp"
#include "graphics/animatedSprite.hpp"
#include "core/console.hpp"
#include "events/mouseEvent.hpp"

// The root is the entire window
UIElement UIManager::s_root;
std::unordered_map<std::string, Ref<UIElement>> UIManager::s_UIElements;
UISerializer* UIManager::s_serializer;
UIBoundVariables * UIManager::s_boundVariables;
UIEventManager * UIManager::s_eventManager;

void UIManager::Init()
{
    // Initialize the font resources used for UI
    Oasis::TextRenderer::LoadFont(GetUIFont(UIFont::DEFAULT), GetUIFontPath(UIFont::DEFAULT), GetUIFontSize(UIFont::DEFAULT));
    Oasis::TextRenderer::LoadFont(GetUIFont(UIFont::SMALL), GetUIFontPath(UIFont::SMALL), GetUIFontSize(UIFont::SMALL));

    // Only some of these matter for the calculation but we set all of them anyways
    s_root.mShow = true;
    s_root.mAnchor = UIAnchor::BOTTOM_LEFT;
    s_root.mWidth = Oasis::WindowService::WindowWidth();
    s_root.mHeight = Oasis::WindowService::WindowHeight();
    s_root.mXOffset = 0;
    s_root.mYOffset = 0;
    s_root.mUIType = UIType::NONE;

    s_serializer = new UISerializer();
    s_boundVariables = new UIBoundVariables();
    s_eventManager = new UIEventManager();

    DeserializeUI();
}

void UIManager::Shutdown()
{
    delete s_eventManager;
    delete s_boundVariables;
    delete s_serializer;
}

void UIManager::Update()
{
    typedef std::function<void(Ref<UIElement>, int, int, int, int)> f;
    f update_ui = [&](Ref<UIElement> curr, int parent_x, int parent_y, int parent_w, int parent_h) {
        OASIS_TRAP(parent_w >= 0 && parent_h >= 0);
        if (!curr->mShow)
        {
            return;
        }
        const unsigned int w = curr->mWidth;
        const unsigned int h = curr->mHeight;
        // Calculate the x/y of our current UI Element
        int x = 0;
        int y = 0;
        switch(curr->mAnchor)
        {
            case UIAnchor::TOP_LEFT: {
                x = parent_x, y = parent_y + parent_h;
            } break;
            case UIAnchor::TOP_RIGHT: {
                x = parent_x + parent_w, y = parent_y + parent_h;
            } break;
            case UIAnchor::BOTTOM_LEFT: {
                x = parent_x, y = parent_y;
            } break;
            case UIAnchor::BOTTOM_RIGHT: {
                x = parent_x + parent_w, y = parent_y;
            } break;
            case UIAnchor::CENTER: {
                x = parent_x + parent_w / 2;
                y = parent_y + parent_h / 2;
            } break;
            default: {
                OASIS_TRAP(false && "UI Anchor should be assigned");
            } break;
        };
        x += curr->mXOffset;
        y += curr->mYOffset;
        switch(curr->mUIType)
        {
            case UIType::NONE: {
                // Do nothing...
            } break;
            case UIType::BACKGROUND: {
                // Draw the background
                Oasis::Renderer::DrawQuad((float) x, (float) y, (float) curr->mWidth, (float) curr->mHeight, curr->mBackground);
                // Draw the border as a line strip
                float * border = new float[curr->mBorderWidth * 5 * 2];
                for (unsigned int i = 0; i < curr->mBorderWidth; ++i)
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
                Oasis::Renderer::DrawLineStrip(border, curr->mBorderWidth * 5, curr->mBorder);
            } break;
            case UIType::TEXT: {
                const int length = Oasis::TextRenderer::DrawString(GetUIFont(curr->mFont), std::string(curr->mText), (float) x, (float) y, curr->mColour);
            } break;
            case UIType::TEXTURE: {
                // Cache the sprite so we don't have to constantly recreate it
                if (!curr->mCachedSprite)
                {
                    curr->mCachedSprite = new Oasis::Sprite(curr->mPath);
                }
                curr->mCachedSprite->SetDimensions((float)curr->mWidth, (float)curr->mHeight);
                curr->mCachedSprite->SetPos((float)x, (float)y);
                Oasis::Renderer::DrawSprite(curr->mCachedSprite);
            } break;
            case UIType::TEXT_DYNAMIC: {
                // TODO: Optimizable with caching
                // Will need to keep track of which variables have become dirty
                // First resolve the format string to see what bound variables we need to set
                OASIS_TRAP(curr->mFormatString);
                std::string originalString(curr->mFormatString);
                std::string resolvedString(curr->mFormatString);
                char * token = curr->mFormatString;
                unsigned int varStartIndex = 0;
                unsigned int counter = 0;
                bool varStarted = false;
                while(*token != '\0')
                {
                    // Start the find here
                    if (*token == '<')
                    {
                        varStartIndex = counter + 1;
                        varStarted = true;
                    }
                    if (*token == '>')
                    {
                        varStarted = false;
                        // variable length
                        const unsigned int len = counter - varStartIndex;
                        std::string var_name = originalString.substr(varStartIndex, len);
                        std::string value = s_boundVariables->GetVariableAsString(var_name);
                        resolvedString.replace(varStartIndex - 1, len + 2, value);
                    }
                    token++;
                    counter++;
                }
                const int length = Oasis::TextRenderer::DrawString(GetUIFont(curr->mFont), resolvedString, (float) x, (float) y, curr->mColour);
            } break;
            case UIType::BUTTON: {
                // Cache the sprites so we don't have to recreate them
                if (!curr->mCachedButtonSprite)
                {
                    curr->mCachedButtonSprite = new Oasis::Sprite(curr->mPath);
                }
                if (!curr->mCachedHoverSprite)
                {
                    curr->mCachedHoverSprite = new Oasis::Sprite(curr->mHoverPath);
                }
                curr->mCachedButtonSprite->SetDimensions((float)curr->mWidth, (float)curr->mHeight);
                curr->mCachedButtonSprite->SetPos((float)x, (float)y);
                curr->mCachedHoverSprite->SetDimensions((float)curr->mWidth, (float)curr->mHeight);
                curr->mCachedHoverSprite->SetPos((float)x, (float)y);
                if (curr->mHovering)
                {
                    Oasis::Renderer::DrawSprite(curr->mCachedHoverSprite);
                }
                else
                {
                    Oasis::Renderer::DrawSprite(curr->mCachedButtonSprite);
                }
            } break;
            case UIType::ANIMATED_TEXTURE: {
                // Cache the sprite so we don't have to constantly recreate it
                if (!curr->mCachedAnimatedSprite)
                {
                    OASIS_TRAP(curr->mAnimFrames > 0 && "Must have at least one animation frame");
                    curr->mCachedAnimatedSprite = new Oasis::AnimatedSprite(curr->mPath, (float)curr->mFrameWidth, (float)curr->mFrameHeight);
                    curr->mCachedAnimatedSprite->AddAnimation("default", 0, curr->mAnimFrames - 1);
                    curr->mCachedAnimatedSprite->SetFPS(curr->mFps);
                    curr->mCachedAnimatedSprite->PlayAnimation("default");
                }
                curr->mCachedAnimatedSprite->SetDimensions((float)curr->mWidth, (float)curr->mHeight);
                curr->mCachedAnimatedSprite->SetPos((float)x, (float)y);
                Oasis::Renderer::DrawAnimatedSprite(curr->mCachedAnimatedSprite);
            } break;
            default: {
                OASIS_TRAP(false && "UI Type should be assigned(Can be set to NONE)");
            } break;
        }
        // Recurse over children
        for (auto child : curr->mChildren)
        {
            update_ui(child, x, y, w, h);
        }
        return;
    };
    // Update window width/height of the root
    s_root.mWidth = Oasis::WindowService::WindowWidth();
    s_root.mHeight = Oasis::WindowService::WindowHeight();
    update_ui(&s_root, 0, 0, Oasis::WindowService::WindowWidth(), Oasis::WindowService::WindowHeight());
}

bool UIManager::HandleEvent(const Oasis::Event& event)
{
    if (event.GetType() == Oasis::EventType::MOUSE_MOVE)
    {
        const Oasis::MouseMovedEvent& move = dynamic_cast<const Oasis::MouseMovedEvent&>(event);
        const int mX = move.GetX();
        const int mY = move.GetY();

        // TODO: A lot of shared code w/ update_ui, maybe can refactor it out somewhere
        typedef std::function<void(Ref<UIElement>, int, int, int, int)> f;
        f update_button = [&](Ref<UIElement> curr, int parent_x, int parent_y, int parent_w, int parent_h) {
            OASIS_TRAP(parent_w >= 0 && parent_h >= 0);
            if (!curr->mShow)
            {
                curr->mHovering = false;
                return;
            }
            const unsigned int w = curr->mWidth;
            const unsigned int h = curr->mHeight;
            // Calculate the x/y of our current UI Element
            int x = 0;
            int y = 0;
            switch(curr->mAnchor)
            {
                case UIAnchor::TOP_LEFT: {
                    x = parent_x, y = parent_y + parent_h;
                } break;
                case UIAnchor::TOP_RIGHT: {
                    x = parent_x + parent_w, y = parent_y + parent_h;
                } break;
                case UIAnchor::BOTTOM_LEFT: {
                    x = parent_x, y = parent_y;
                } break;
                case UIAnchor::BOTTOM_RIGHT: {
                    x = parent_x + parent_w, y = parent_y;
                } break;
                case UIAnchor::CENTER: {
                    x = parent_x + parent_w / 2;
                    y = parent_y + parent_h / 2;
                } break;
                default: {
                    OASIS_TRAP(false && "UI Anchor should be assigned");
                } break;
            };
            x += curr->mXOffset;
            y += curr->mYOffset;
            if (curr->mUIType== UIType::BUTTON)
            {
                if (mX > x && mX < x + (int)w && mY > y && mY < y + (int)h)
                {
                    curr->mHovering = true;
                }
                else
                {
                    curr->mHovering = false;
                }
            }
            
            for (auto child : curr->mChildren)
            {
                update_button(child, x, y, w, h);
            }
            return;
        };
        // Check collisions and update if we are hovering on the button
        update_button(&s_root, 0, 0, Oasis::WindowService::WindowWidth(), Oasis::WindowService::WindowHeight());
        return false;
    }
    if (event.GetType() == Oasis::EventType::MOUSE_PRESS)
    {
        typedef std::function<bool(Ref<UIElement>)> f;
        f click_button = [&](Ref<UIElement> curr) {
            if (!curr->mShow)
            {
                return false;
            }
            if (curr->mUIType == UIType::BUTTON && curr->mHovering)
            {
                s_eventManager->OnEvent(curr->mClickEvent);
                return true;
            }
            for (auto child : curr->mChildren)
            {
                bool result = click_button(child);
                if (result) return true;
            }
            return false;
        };

        // Assume the hovering info is recent and accurate enough
        bool result = click_button(&s_root);
        return result;
    }
    return false;
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

void UIManager::ShowWindow(const std::string& name)
{
    if (auto element = GetUIElement(name))
    {
        element->mShow = true;   
    }
    else
    {
        // TODO: Can probably stuff this in the GetUIElement function
        Oasis::Console::Error("Couldn't find UI element: %s", name.c_str());
    }
}

void UIManager::HideWindow(const std::string& name)
{
    if (auto element = GetUIElement(name))
    {
        element->mShow = false;   
    }
    else
    {
        Oasis::Console::Error("Couldn't find UI element: %s", name.c_str());
    }
}

void UIManager::ToggleWindow(const std::string& name)
{
    if (auto element = GetUIElement(name))
    {
        element->mShow = !element->mShow;   
    }
    else
    {
        Oasis::Console::Error("Couldn't find UI element: %s", name.c_str());
    }
}

void UIManager::SetBoundVariableInt(const std::string& name, int val)
{
    s_boundVariables->SetVariableInt(name, val);
}

void UIManager::SetBoundVariableUInt(const std::string& name, unsigned int val)
{
    s_boundVariables->SetVariableUint(name, val);
}

void UIManager::SetBoundVariableStr(const std::string& name, const std::string& val)
{
    s_boundVariables->SetVariableStr(name, val);
}

void UIManager::AddUIEventCallback(const std::string& event_key, std::function<void()> callback)
{
    s_eventManager->AddListener(event_key, callback);
}

void UIManager::DeserializeUI()
{
    /*
    for (const auto & entry : std::filesystem::directory_iterator(UIManager::Path()))
    {
        if (entry.path().extension() != ".ui")
        {
            continue;
        }
        auto data = s_serializer->Deserialize(entry.path().string(), &s_root);
        for (const auto it : data.mUIElements)
        {
            if (s_UIElements.find(it.first) == s_UIElements.end())
            {
                s_UIElements[it.first] = it.second;
            }
            else
            {
                // We have 2 UI elements with the same name somewhere, BAD
                Oasis::Console::Error("UI Element with name %s already defined, deleting duplicate...", it.first.c_str());
                // Remove from the root and delete it
                for (auto root_it = s_root.mChildren.begin(); root_it != s_root.mChildren.end(); root_it++)
                {
                    if (*root_it == it.second)
                    {
                        s_root.mChildren.erase(root_it);
                        break;
                    }
                }
                delete it.second;
            }
        }
    }
    */
}