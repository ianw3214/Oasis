#pragma once

namespace Oasis
{
    class IState;
    class StateManager
    {
    public:
        static void Init(IState * state);

        static IState * CurrentState() { return m_currentState; }
        static void ChangeState(IState * newState);

    private:
        static IState * m_currentState;
    };
}