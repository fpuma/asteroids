#pragma once

#include <asteroids/flow/states/gamestate.h>

namespace ast
{
    class MenuState : public AsteroidsGameState
    {
    public:

        MenuState();
        virtual ~MenuState() = default;

        void onEnter( StateMachineInfo& _info ) override;
        void update( StateMachineInfo& _info ) override;
        void onExit( StateMachineInfo& _info ) override;

    };
}