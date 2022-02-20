#pragma once

#include "events/event.hpp"
#include "util/util.hpp"

#include <string>
#include <vector>

namespace Oasis
{
    class Entity;
    class Scene
    {
    public:
        void Init();
        void Close();

        void OnEvent(const Event& event);
        void Update();

        void AddEntity(Entity* entity);
    private:
        std::vector<Entity*> mEntities;

    public:
        static Scene* loadFromYAMLtext(const std::string& text);
        static Scene* loadFromFile(const std::string& fileName);
    };
}