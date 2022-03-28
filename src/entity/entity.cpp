#include "entity.hpp"
#include "components/component.hpp"
using namespace Oasis;

Entity::Entity() 
    : mName()
{

}

bool Entity::addComponent(Component* component) {
    mComponents.push_back(component);
    return true;
}

Entity* Entity::loadFromYAML(const ryml::NodeRef tree) {
    // TODO: Input sanitation
    if (tree.has_child("name") && tree["name"].is_keyval()) {
        tree["name"] >> mName;
    }

    if (tree["components"].is_map()) {
        for (const ryml::NodeRef& node : tree["components"]) {
            std::string componentName;
            c4::from_chars(node.key(), &componentName);
            Component * component = ComponentManager::loadFromYAML(componentName, node, this);
            addComponent(component);
        }
    }
    return this;
}