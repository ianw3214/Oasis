#pragma once

#include <ryml.hpp>

class YAMLParser {
public:
    static void ParseStr(const ryml::NodeRef tree, const std::string& key, std::string& target, const std::string& default = "");
};