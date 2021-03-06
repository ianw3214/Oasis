#include "gameState.hpp"
using namespace Oasis;

void GameState::Init() 
{
    CreateLayers();
    for (GameStateLayer * layer : m_layers)
    {
        layer->Init();
    }
}

void GameState::Close()
{
    for (auto& layer : m_layers)
    {
        layer->Close();
        delete layer;
    }
}

void GameState::OnEvent(const Event& event)
{
    // Use a reverse iterator so we can handle top layer to bottom order
    for(auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
    {
        bool handled = (*it)->HandleEvent(event);
        if (handled) break;
    }
}

void GameState::Update()
{
    for(auto& layer : m_layers)
    {
        layer->Update();
    }
}


void GameState::AddLayer(GameStateLayer * layer)
{
    m_layers.push_back(layer);
}