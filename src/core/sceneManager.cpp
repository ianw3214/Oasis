#include "sceneManager.hpp"
#include "scene.hpp"
using namespace Oasis;

#include "util/trap.hpp"

// TODO: Actual file manager
#include <fstream>
#include <sstream>
#include <ryml_std.hpp>
#include <ryml.hpp>

Scene* SceneManager::mCurrentScene;

void SceneManager::Init(const std::string& file)
{
    // Load config file
    std::ifstream config(file);
    std::stringstream buffer;
    buffer << config.rdbuf();
    std::string fileData = buffer.str();
    ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(fileData));

    // Parse relevant config data
    int width = 500;
    if (tree["width"].is_keyval()) {
        tree["width"] >> width;
    }

    int height = 500;
    if (tree["height"].is_keyval()) {
        tree["height"] >> height;
    }

    std::string name;
    if (tree["name"].is_keyval()) {
        tree["name"] >> name;
    }

    std::string sceneFile;
    if (tree["starting_scene"].is_keyval()) {
        tree["starting_scene"] >> sceneFile;
    }

    Scene* scene = Scene::loadFromFile(sceneFile);
    // TODO: Proper error handling instead of trap
    OASIS_TRAP(scene);
    mCurrentScene = scene;
}

void SceneManager::ChangeScene(Scene* newScene)
{
    OASIS_TRAP(newScene);
    if (mCurrentScene) mCurrentScene->Close();
    delete mCurrentScene;
    mCurrentScene = newScene;
    mCurrentScene->Init();
}