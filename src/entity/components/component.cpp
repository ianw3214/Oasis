#include "component.hpp"
using namespace Oasis;

#include "core/renderComponent.hpp"

std::unordered_map<std::string, ComponentManager::LoadFunc> ComponentManager::mComponentMap;

void ComponentManager::InitCoreComponents() {
    registerComponent("render", RenderComponent::loadFromYAML);
}

Component* ComponentManager::loadFromYAML(const std::string& name, const ryml::NodeRef tree, Entity* owner) {
    auto funcIt = mComponentMap.find(name);
    if (funcIt == mComponentMap.end()) {
        return nullptr;
    }
    return (funcIt->second)(tree, owner);
}

bool ComponentManager::registerComponent(const std::string& name, LoadFunc func) {
    if (mComponentMap.find(name) != mComponentMap.end()) {
        return false;
    }
    mComponentMap[name] = func;
    return true;
}

Component::Component(Entity* owner) 
    : mOwner(owner)
{

}

Component* Component::loadFromYAML(const ryml::NodeRef tree, Entity* owner) {
    return new Component(owner);
}