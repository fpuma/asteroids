#include <precompiledengine.h>

#include "menustate.h"

#include <asteroids/flow/layers/menulayer.h>
#include <asteroids/flow/layers/base/layermanager.h>
#include <asteroids/fakedata/data.h>

#include <engine/ecs/components/iinputcomponent.h>
#include <engine/services/ecsservice.h>

namespace ast
{
    MenuState::MenuState()
        : AsteroidsGameState( gData->kGameStates.MenuState )
    {

    }

    void MenuState::onEnter( StateMachineInfo& _info )
    {
        gLayerManager->addLayer( std::make_unique<MenuLayer>() );
    }

    void MenuState::update( StateMachineInfo& _info )
    {
        MenuLayer* menuLayer = gLayerManager->getLayer<MenuLayer>( gData->kGameLayers.MenuLayer );

        Entity menuControllerEntity = menuLayer->getMenuControllerEntity();

        IInputComponent* inputComponent = gComponents->getComponent<IInputComponent>( menuControllerEntity );

        if (inputComponent->isActionActive( gData->kInputActions.MenuAButton ))
        {
            _info.currentStateId = gData->kGameStates.GameplayState;
        }
    }

    void MenuState::onExit( StateMachineInfo& _info )
    {
        gLayerManager->removeLayer( gData->kGameLayers.MenuLayer );
    }
}