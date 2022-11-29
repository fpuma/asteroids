#pragma once

#include <modules/pina/entity.h>
#include <engine/igame.h>
#include <asteroids/fakedata/data.h>
#include <utils/statemachine/statemachine.h>
#include <asteroids/flow/states/gamestate.h>

using namespace puma;
namespace ast
{
    class Asteroids : public IGame
    {
    public:

        void init() override;
        void uninit() override;
        void update( float _deltaTime ) override;

    private:

        Entity m_backgroundEntity;

        StatelessMachine<AsteroidsGameState> m_stateMachine;
        StateMachineInfo m_stateMachineInfo;

        std::unique_ptr<Data> m_data;
    };
}