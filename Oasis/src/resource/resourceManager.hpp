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
        static Reference<T> LoadResource(const std::string& path);
        
        template<class T>
        static Reference<T> GetResource(const std::string& path);

        static void UnloadResource(const std::string& path);
    private:
        static ResourceManager * GetInstance();
        std::unordered_map<std::string, Owned<Resource>> m_resources;
    };
}

namespace Oasis
{
    template<class T>
    Reference<T> ResourceManager::LoadResource(const std::string& path)
    {
        GetInstance()->m_resources[path] = T::Load(path);
        return Reference(dynamic_cast<T*>(GetInstance()->m_resources[path].GetData()));
    }

    template<class T>
    Reference<T> ResourceManager::GetResource(const std::string& path)
    {
        if (GetInstance()->m_resources.find(path) == GetInstance()->m_resources.end())
        {
            LoadResource<T>(path);
        }
        return Reference(dynamic_cast<T*>(GetInstance()->m_resources[path].GetData()));
    }
}