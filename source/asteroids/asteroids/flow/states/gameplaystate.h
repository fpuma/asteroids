#pragma once

#include <asteroids/flow/states/gamestate.h>

namespace ast
{
    class GameplayState : public AsteroidsGameState
    {
    public:

        GameplayState();
        virtual ~GameplayState() = default;

        void onEnter( StateMachineInfo& _info ) override;
        void update( StateMachineInfo& _info ) override;
        void onExit( StateMachineInfo& _info ) override;

    private:
    };
}