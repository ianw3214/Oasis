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

        // This is very bad
        // TODO: Find a better way to do this
        template<typename T>
        T * getComponent() {
            for (Component* component : mComponents) {
                T* cast = dynamic_cast<T*>(component);
                if (cast) return cast;
            }
            return nullptr;
        }
    private:
        std::string mName;
        // TODO: Better place to store this
        std::vector<Component*> mComponents;
    };
}