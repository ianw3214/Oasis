#include "systemManager.hpp"
#include "system.hpp"
using namespace Oasis;

#include "core/sceneManager.hpp"
#include "core/scene.hpp"

#include "core/renderSystem.hpp"

std::vector<System*> SystemManager::mSystems;

void SystemManager::Init() {
    SystemManager::RegisterSystem(new DummySystem());
    SystemManager::RegisterSystem(new RenderSystem());
}

void SystemManager::Update(Scene* scene) {
    for (System* system : SystemManager::mSystems) {
        system->Update(scene->getEntitiesMutable());
    }
}
