#include <precompiledengine.h>

#include "gameplaystate.h"

#include <asteroids/flow/layers/base/layermanager.h>
#include <asteroids/flow/layers/gameplaylayer.h>
#include <asteroids/systems/shipmovementsystem.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>

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
        ShipMovementSystem* shipSystem = gSystems->getSystem<ShipMovementSystem>();
        Entity shipEntity = shipSystem->getShipEntity();

        if (!gEntities->isEntityEnabled( shipEntity ))
        {
            _info.currentStateId = gData->kGameStates.MenuState;
        }

    }

    void GameplayState::onExit( StateMachineInfo& _info )
    {
        gLayerManager->removeLayer( gData->kGameLayers.GameplayLayer );
    }
}