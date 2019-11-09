#pragma once

namespace Oasis
{
    class IState
    {
    public:
        virtual void Init() = 0;
        virtual void Close() = 0;

        virtual void Update() = 0;
    };
};