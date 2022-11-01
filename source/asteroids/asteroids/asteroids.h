#pragma once

#include <modules/pina/entity.h>
#include <engine/igame.h>
#include <asteroids/fakedata/data.h>
#include <utils/statemachine/statemachine.h>

using namespace puma;
namespace ast
{


    class AsteroidsState : public IState<StateMachineInfo>
    {
    public:

        AsteroidsState( StateId _id ) : IState( _id ) {}
    };

    class Asteroids : public IGame
    {
    public:

        void init() override;
        void uninit() override;
        void update( float _deltaTime ) override;

    private:

        Entity m_backgroundEntity;

        std::unique_ptr<Data> m_data;
    };
}