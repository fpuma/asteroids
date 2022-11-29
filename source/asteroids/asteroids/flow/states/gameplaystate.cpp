#include <precompiledengine.h>

#include "gameplaystate.h"

#include <asteroids/flow/layers/base/layermanager.h>
#include <asteroids/flow/layers/gameplaylayer.h>

#include <asteroids/fakedata/data.h>

using namespace puma;

namespace ast
{
    GameplayState::GameplayState()
        : AsteroidsGameState( gData->kGameStates.GameplayState )
    {

    }

    void GameplayState::onEnter( StateMachineInfo& _info )
    {
        gLayerManager->addLayer( std::make_unique<GameplayLayer>() );
    }

    void GameplayState::update( StateMachineInfo& _info )
    {

    }

    void GameplayState::onExit( StateMachineInfo& _info )
    {
        gLayerManager->removeLayer( gData->kGameLayers.GameplayLayer );
    }
}