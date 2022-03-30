#include "yamlParser.hpp"

void YAMLParser::ParseStr(const ryml::NodeRef tree, const std::string& key, std::string& target, const std::string& default) {
    c4::csubstr keyStr = ryml::to_csubstr(key);
    if (tree.has_child(keyStr) && tree[keyStr].is_keyval()) {
        tree["path"] >> target;
    } else {
        target = default;
    }
}