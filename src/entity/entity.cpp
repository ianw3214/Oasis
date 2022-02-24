#include "entity.hpp"
#include "components/component.hpp"
using namespace Oasis;

Entity::Entity(const std::string& name) 
    : mName(name)
{

}

bool Entity::addComponent(Component* component) {
    mComponents.push_back(component);
    return true;
}

Entity* Entity::loadFromYAML(const ryml::NodeRef tree) {
    std::string name;
    if (tree["name"].is_keyval()) {
        tree["name"] >> name;
    }
    Entity * result = new Entity(name);
    if (tree["components"].is_map()) {
        for (ryml::NodeRef node : tree["components"]) {
            std::string componentName;
            c4::from_chars(node.key(), &componentName);
            Component * component = ComponentManager::loadFromYAML(componentName, node);
            result->addComponent(component);
        }
    }
    return result;
}