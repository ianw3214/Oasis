#pragma once

namespace Oasis
{
    class Event;
    class IState
    {
    public:
        virtual void Init() = 0;
        virtual void Close() = 0;

        virtual void OnEvent(const Event& event) = 0;
        virtual void Update() = 0;
    };
};