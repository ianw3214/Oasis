#pragma once

#include <string>
#include <unordered_map>

#include "resource.hpp"

namespace Oasis
{
    class ResourceManager
    {
    public:
        static void Init();
        static void Shutdown();

        template<class T>
        static T * LoadResource(const std::string& path);
        
        template<class T>
        static T * GetResource(const std::string& path);

        static void UnloadResource(const std::string& path);
    private:
        static ResourceManager * GetInstance();
        std::unordered_map<std::string, Resource*> m_resources;
    };
}

namespace Oasis
{
    template<class T>
    T * ResourceManager::LoadResource(const std::string& path)
    {
        if (GetInstance()->m_resources[path])
        {
            delete GetInstance()->m_resources[path];
        }
        GetInstance()->m_resources[path] = T::Load(path);
        return dynamic_cast<T*>(GetInstance()->m_resources[path]);
    }

    template<class T>
    T * ResourceManager::GetResource(const std::string& path)
    {
        if (GetInstance()->m_resources.find(path) != GetInstance()->m_resources.end())
        {
            return dynamic_cast<T*>(GetInstance()->m_resources[path]);
        }
        LoadResource<T>(path);
        return dynamic_cast<T*>(GetInstance()->m_resources[path]);
    }
}