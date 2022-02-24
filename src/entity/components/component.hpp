#pragma once

#include <ryml.hpp>

#include <string>
#include <unordered_map>
#include <functional>

namespace Oasis 
{
    class Component;
    class ComponentManager
    {
    public:
        typedef std::function<Component*(const ryml::NodeRef)> LoadFunc;
        static Component* loadFromYAML(const std::string& name, const ryml::NodeRef tree);
        static bool registerComponent(const std::string& name, LoadFunc func);
    private:
        static std::unordered_map<std::string, LoadFunc> mComponentMap;
    };

    class Component 
    {
    public:
        static Component* loadFromYAML(const ryml::NodeRef tree);
    public:
        Component(const std::string& name);
    private:
        std::string mName;
    };
}