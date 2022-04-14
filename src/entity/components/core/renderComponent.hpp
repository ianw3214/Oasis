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
    Oasis::Sprite* getSprite() const { return mSprite; }

    float getX() const { return mX; }
    float getY() const { return mY; }
    void setPos(float x, float y) { mX = x; mY = y; }
private:
    std::string mPath;

    float mX;
    float mY;
    Oasis::Sprite* mSprite;
};