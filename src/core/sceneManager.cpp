#include "sceneManager.hpp"
#include "scene.hpp"
using namespace Oasis;

#include "util/trap.hpp"

Scene* SceneManager::mCurrentScene;

void SceneManager::Init(Scene* scene)
{
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