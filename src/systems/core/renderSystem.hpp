#pragma once

#include "systems/system.hpp"

class RenderSystem : public Oasis::System {
public:
    void Update(std::vector<Oasis::Entity*>& entities) override;
};