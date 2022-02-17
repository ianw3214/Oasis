#pragma once

#include <string>
#include <unordered_map>

#include "util/util.hpp"

#include "resource/resource.hpp"

namespace Oasis
{
    class ResourceManager
    {
    public:
        static void Init();
        static void Shutdown();

        template<class T>
        static Ref<T> LoadResource(const std::string& path);
        
        template<class T>
        static Ref<T> GetResource(const std::string& path);

        static void UnloadResource(const std::string& path);
    private:
        static ResourceManager * GetInstance();
        std::unordered_map<std::string, Resource*> m_resources;
    };
}

namespace Oasis
{
    template<class T>
    Ref<T> ResourceManager::LoadResource(const std::string& path)
    {
        // Check if resource has already been loaded
        if (GetInstance()->m_resources.find(path) == GetInstance()->m_resources.end())
        {
            GetInstance()->m_resources[path] = T::Load(path);
        }
        return dynamic_cast<T*>(GetInstance()->m_resources[path]);
    }

    template<class T>
    Ref<T> ResourceManager::GetResource(const std::string& path)
    {
        if (GetInstance()->m_resources.find(path) == GetInstance()->m_resources.end())
        {
            LoadResource<T>(path);
        }
        return dynamic_cast<T*>(GetInstance()->m_resources[path]);
    }
}