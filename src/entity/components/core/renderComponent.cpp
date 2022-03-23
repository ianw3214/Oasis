#include "renderComponent.hpp"

#include "graphics/sprite.hpp"

Oasis::Component* RenderComponent::loadFromYAML(const ryml::NodeRef tree, Oasis::Entity* owner) {
    RenderComponent* result = new RenderComponent(owner);
    std::string path;
    if (tree["path"].is_keyval()) {
        tree["path"] >> path;
        result->setPath(path);
        result->mSprite = new Oasis::Sprite(path);
    }
    return result;
}

RenderComponent::RenderComponent(Oasis::Entity* owner) : Component(owner) {
    
}