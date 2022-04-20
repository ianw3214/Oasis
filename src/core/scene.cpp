#include "scene.hpp"
using namespace Oasis;

#include "entity/entity.hpp"
#include "graphics/renderer.hpp"
#include "graphics/sprite.hpp"

// TODO: Actual file manager
#include <fstream>
#include <sstream>
#include <ryml_std.hpp>
#include <ryml.hpp>

#include <algorithm>

void Scene::Init() 
{

}

void Scene::Close()
{
}

void Scene::OnEvent(const Event& event)
{
}

void Scene::Update()
{
    std::sort(mSceneObjects.begin(), mSceneObjects.end(), [](SceneObject* a, SceneObject* b) {
        return a->mLayer < b->mLayer;
    });

    for (SceneObject* sceneObject : mSceneObjects) {
        sceneObject->mSprite->SetPos((float)sceneObject->x, (float)sceneObject->y);
        Oasis::Renderer::DrawSprite(sceneObject->mSprite);
    }
}

void Scene::AddEntity(Entity* entity) 
{
    mEntities.push_back(entity);
}

SceneObject* Scene::InsertSceneObject(Oasis::Sprite* sprite, int layer) {
    mSceneObjects.push_back(new SceneObject(sprite, layer));
    return mSceneObjects[mSceneObjects.size() - 1];
}

bool Scene::loadFromYAMLtext(const std::string& text) {
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(text));
    for (ryml::NodeRef node : tree["entities"]) {
        Entity* entity = new Entity();
        entity->loadFromYAML(node);
        AddEntity(entity);
    }
    return true;
}

bool Scene::loadFromFile(const std::string& fileName) {
    // Load scene file
    // TODO: Handle file not found
    std::ifstream config(fileName);
    std::stringstream buffer;
    buffer << config.rdbuf();
    std::string fileData = buffer.str();
    return Scene::loadFromYAMLtext(fileData);
}
