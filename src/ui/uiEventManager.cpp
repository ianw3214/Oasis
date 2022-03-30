#include "uiEventManager.hpp"

#include "core/console.hpp"

UIEventManager::UIEventManager()
{

}

void UIEventManager::OnEvent(const std::string& event_key)
{
    auto it = mCallbacks.find(event_key);
    if (it != mCallbacks.end())
    {
        for (auto callback : it->second)
        {
            callback();
        }
    }
}

void UIEventManager::AddListener(const std::string& event_key, std::function<void()> callback)
{
    if (mCallbacks.find(event_key) == mCallbacks.end())
    {
        mCallbacks[event_key] = std::vector<std::function<void()>>();
    }
    mCallbacks[event_key].push_back(callback);
}
