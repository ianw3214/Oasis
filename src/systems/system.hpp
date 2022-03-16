#pragma once

#include <vector>

namespace Oasis
{
    class Entity;
    class System
    {
    public:
        virtual void Update(std::vector<Entity*>& entities) {}
    };
}

class DummySystem : public Oasis::System {
public:
    void Update(std::vector<Oasis::Entity*>& entities) override;
};