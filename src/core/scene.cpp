#include "scene.hpp"
using namespace Oasis;

#include "entity/entity.hpp"

// TODO: Actual file manager
#include <fstream>
#include <sstream>
#include <ryml_std.hpp>
#include <ryml.hpp>

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
}

void Scene::AddEntity(Entity* entity) 
{
    mEntities.push_back(entity);
}

Scene* Scene::loadFromYAMLtext(const std::string& text) {
    Scene* scene = new Scene();

    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(text));
    for (ryml::NodeRef node : tree["entities"]) {
        Entity* entity = Entity::loadFromYAML(node);
        scene->AddEntity(entity);
    }
    return scene;
}

Scene* Scene::loadFromFile(const std::string& fileName) {
    // Load scene file
    // TODO: Handle file not found
    std::ifstream config(fileName);
    std::stringstream buffer;
    buffer << config.rdbuf();
    std::string fileData = buffer.str();
    return Scene::loadFromYAMLtext(fileData);
}
