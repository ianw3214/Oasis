#pragma once

#include <string>
#include <ryml.hpp>

#include <string>

namespace Oasis 
{
    class Entity 
    {
    public:
        static Entity* loadFromYAML(const ryml::NodeRef tree);
    public:
        Entity(const std::string& name);
    private:
        std::string mName;
    };
}