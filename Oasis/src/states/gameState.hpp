#pragma once

#include "util/util.hpp"
#include "core/state.hpp"

#include <vector>

namespace Oasis
{
    //////////////////////////////////////////////////
    class GameStateLayer
    {
    public:
        virtual void Init() = 0;
        virtual void Close() = 0;

        virtual bool HandleEvent(const Event& event) = 0;
        virtual void Update() = 0;
    };

    //////////////////////////////////////////////////
    class GameState : public IState
    {
    public:
        virtual void Init() override;
        virtual void Close() override;

        virtual void InitLayers() = 0;

        virtual void OnEvent(const Event& event) override;
        virtual void Update() override;

        void AddLayer(GameStateLayer * layer);
    private:
        std::vector<Owned<GameStateLayer>> m_layers;
    };  
}