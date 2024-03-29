#include "systemManager.hpp"
#include "system.hpp"
using namespace Oasis;

#include "core/application.hpp"
#include "core/sceneManager.hpp"
#include "core/scene.hpp"

std::vector<System*> SystemManager::mSystems;

void SystemManager::Init(const Configuration& config) {
    SystemManager::RegisterSystem(new DummySystem());

    for (System* system : config.mSystems) {
        SystemManager::RegisterSystem(system);
    }
}

void SystemManager::Update(Scene* scene) {
    for (System* system : SystemManager::mSystems) {
        system->Update(scene->getEntitiesMutable());
    }
}
