#include "entity.hpp"
using namespace Oasis;

Entity::Entity(const std::string& name) 
    : mName(name)
{

}

Entity* Entity::loadFromYAML(const ryml::NodeRef tree) {
    std::string name;
    if (tree["name"].is_keyval()) {
        tree["name"] >> name;
    }
    return new Entity(name);
}