#pragma once

#include "events/event.hpp"
#include "util/util.hpp"

#include <string>

namespace Oasis
{
    class Scene
    {
    public:
        void Init();
        void Close();

        void OnEvent(const Event& event);
        void Update();

    public:
        static Scene* loadFromYAMLtext(const std::string& text);
        static Scene* loadFromFile(const std::string& fileName);
    };
}