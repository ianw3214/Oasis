#include "renderComponent.hpp"

#include "file/yamlParser.hpp"
#include "graphics/sprite.hpp"

Oasis::Component* RenderComponent::loadFromYAML(const ryml::NodeRef tree, Oasis::Entity* owner) {
    // TODO: Separate into render definition and render component
    //  - Initializing sprite should be done in component init, not yaml loading
    RenderComponent* result = new RenderComponent(owner);
    std::string path;
    YAMLParser::ParseStr(tree, "path", path);
    result->setPath(path);
    result->mSprite = new Oasis::Sprite(path);
    return result;
}

RenderComponent::RenderComponent(Oasis::Entity* owner) 
    : Component(owner) 
    , mX(0.f) 
    , mY(0.f)
    , mSprite(nullptr)
{
    
}