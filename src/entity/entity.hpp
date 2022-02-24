#pragma once

#include <ryml.hpp>

#include <string>
#include <vector>

namespace Oasis 
{
    class Component;
    class Entity 
    {
    public:
        static Entity* loadFromYAML(const ryml::NodeRef tree);
    public:
        Entity(const std::string& name);
        bool addComponent(Component* component);
    private:
        std::string mName;
        // TODO: Better place to store this
        std::vector<Component*> mComponents;
    };
}