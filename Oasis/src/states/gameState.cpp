#include "gameState.hpp"
using namespace Oasis;

void GameState::Init() 
{
    InitLayers();
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
    for(auto& layer : m_layers)
    {
        bool handled = layer->HandleEvent(event);
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
    layer->Init();
    m_layers.push_back(layer);
}