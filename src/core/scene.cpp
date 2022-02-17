#include "scene.hpp"
using namespace Oasis;

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

Scene* Scene::loadFromYAMLtext(const std::string& text) {
    return new Scene();
}

Scene* Scene::loadFromFile(const std::string& fileName) {
    return new Scene();
}
