#pragma once

#include <string>
#include <vector>

namespace Oasis
{
    struct Configuration;

    class System;
    class Scene;
    class SystemManager
    {
    public:
        static void Init(const Configuration& config);
        static void RegisterSystem(System* system) {
            mSystems.push_back(system);
        }

        static void Update(Scene* scene);
    private:
        static std::vector<System*> mSystems;
    };
}