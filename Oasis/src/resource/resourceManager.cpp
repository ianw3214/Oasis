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
    delete GetInstance()->m_resources[path];
}