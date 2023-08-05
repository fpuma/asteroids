#include <precompiledengine.h>

#include "gameplaystate.h"

#include <asteroids/components/shipcomponent.h>
#include <asteroids/flow/layers/base/layermanager.h>
#include <asteroids/flow/layers/gameplaylayer.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
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
        auto shipEntities = gECS->getEntitesByComponents<ShipComponent>();
        pina::Entity shipEntity;

        if (shipEntities.empty())
        {
            _info.currentStateId = gData->kGameStates.MenuState;
        }
    }

    void GameplayState::onExit( StateMachineInfo& _info )
    {
        gLayerManager->removeLayer( gData->kGameLayers.GameplayLayer );
    }
}