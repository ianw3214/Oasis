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
        std::unordered_map<std::string, Resource*> mResources;
    };
}

namespace Oasis
{
    template<class T>
    Ref<T> ResourceManager::LoadResource(const std::string& path)
    {
        // Check if resource has already been loaded
        if (GetInstance()->mResources.find(path) == GetInstance()->mResources.end())
        {
            GetInstance()->mResources[path] = T::Load(path);
        }
        return dynamic_cast<T*>(GetInstance()->mResources[path]);
    }

    template<class T>
    Ref<T> ResourceManager::GetResource(const std::string& path)
    {
        if (GetInstance()->mResources.find(path) == GetInstance()->mResources.end())
        {
            LoadResource<T>(path);
        }
        return dynamic_cast<T*>(GetInstance()->mResources[path]);
    }
}