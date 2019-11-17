#pragma once

namespace Oasis
{
    class Event;
    class ImGuiWrapper
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnEvent(const Event& event);
        static void Update(float deltaTime);
    };
}