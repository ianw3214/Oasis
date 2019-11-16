#include "resourceManager.hpp"
using namespace Oasis;

void ResourceManager::Init()
{

}

void ResourceManager::Shutdown()
{

}

ResourceManager * ResourceManager::GetInstance()
{
    static ResourceManager instance;
    return &instance;
}

void ResourceManager::UnloadResource(const std::string& path)
{
    GetInstance()->m_resources[path] = Owned<Resource>();
}