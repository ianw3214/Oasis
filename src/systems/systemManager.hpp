#pragma once

#include <string>
#include <vector>

namespace Oasis
{
    class System;
    class Scene;
    class SystemManager
    {
    public:
        static void Init();
        static void RegisterSystem(System* system) {
            mSystems.push_back(system);
        }

        static void Update(Scene* scene);
    private:
        static std::vector<System*> mSystems;
    };
}