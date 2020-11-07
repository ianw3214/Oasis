#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

// The UI Manager will own an instance of this class, so NOT a static class
class UIEventManager
{
public:
    UIEventManager();

    void OnEvent(const std::string& event_key);
    void AddListener(const std::string& event_key, std::function<void()> callback);
private:
    std::unordered_map<std::string, std::vector<std::function<void()>>> m_callbacks;
};