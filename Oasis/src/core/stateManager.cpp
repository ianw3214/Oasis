#include "stateManager.hpp"
#include "state.hpp"
using namespace Oasis;

#include "util/trap.hpp"

IState * StateManager::m_currentState;

void StateManager::Init(IState * state)
{
    OASIS_TRAP(state);
    m_currentState = state;
}

void StateManager::ChangeState(IState * newState)
{
    OASIS_TRAP(newState);
    if (m_currentState) m_currentState->Close();
    delete m_currentState;
    m_currentState = newState;
    m_currentState->Init();
}