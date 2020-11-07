#include "uiEventManager.hpp"

#include "core/console.hpp"

UIEventManager::UIEventManager()
{

}

void UIEventManager::OnEvent(const std::string& event_key)
{
    auto it = m_callbacks.find(event_key);
    if (it != m_callbacks.end())
    {
        for (auto callback : it->second)
        {
            callback();
        }
    }
}

void UIEventManager::AddListener(const std::string& event_key, std::function<void()> callback)
{
    if (m_callbacks.find(event_key) == m_callbacks.end())
    {
        m_callbacks[event_key] = std::vector<std::function<void()>>();
    }
    m_callbacks[event_key].push_back(callback);
}
