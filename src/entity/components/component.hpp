#pragma once

#include <ryml.hpp>

#include <string>
#include <unordered_map>
#include <functional>

namespace Oasis 
{
    class Entity;
    class Component;
    class ComponentManager
    {
    public:
        static void InitCoreComponents();

        typedef std::function<Component*(const ryml::NodeRef, Entity* owner)> LoadFunc;
        static Component* loadFromYAML(const std::string& name, const ryml::NodeRef tree, Entity* owner);
        static bool registerComponent(const std::string& name, LoadFunc func);
    private:
        static std::unordered_map<std::string, LoadFunc> mComponentMap;
    };

    class Component 
    {
    public:
        static Component* loadFromYAML(const ryml::NodeRef tree, Entity* owner);
    public:
        Component(Entity* owner);
        virtual ~Component() {}
    private:
        Entity* mOwner;
    };
}