#include "system.hpp"
#include "entity/entity.hpp"

#include <Windows.h>

void DummySystem::Update(std::vector<Oasis::Entity*>& entities) {
    OutputDebugStringA("TEST\n");
}