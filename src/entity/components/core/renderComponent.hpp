#pragma once

#include "entity/components/component.hpp"

namespace Oasis {
    class Entity;
    class Sprite;
};
class RenderComponent : public Oasis::Component
{
public:
    static Oasis::Component* loadFromYAML(const ryml::NodeRef tree, Oasis::Entity* owner);
public:
    RenderComponent(Oasis::Entity* owner);

    void setPath(const std::string& path) { mPath = path; }
private:
    std::string mPath;
    Oasis::Sprite* mSprite;
};