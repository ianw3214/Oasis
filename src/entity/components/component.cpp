#include "component.hpp"
using namespace Oasis;

std::unordered_map<std::string, ComponentManager::LoadFunc> ComponentManager::mComponentMap;

Component* ComponentManager::loadFromYAML(const std::string& name, const ryml::NodeRef tree) {
    return nullptr;
}

bool ComponentManager::registerComponent(const std::string& name, LoadFunc func) {
    return true;
}

Component::Component(const std::string& name) 
    : mName(name)
{

}

Component* Component::loadFromYAML(const ryml::NodeRef tree) {
    std::string name;
    if (tree["name"].is_keyval()) {
        tree["name"] >> name;
    }
    return new Component(name);
}