#include "stateManager.hpp"
#include "state.hpp"
using namespace Oasis;

IState * StateManager::m_currentState;

void StateManager::Init(IState * state)
{
    if (state == nullptr)
    {
        // TODO: ERROR HANDLING
    }
    m_currentState = state;
}