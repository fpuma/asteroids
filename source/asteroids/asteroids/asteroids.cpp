#include <precompiledengine.h>
#include "asteroids.h"


#include <asteroids/components/shipcomponent.h>
#include <asteroids/components/shootcomponent.h>
#include <asteroids/components/impactcomponent.h>
#include <asteroids/flow/states/menustate.h>
#include <asteroids/flow/states/gameplaystate.h>
#include <asteroids/systems/shipmovementsystem.h>
#include <asteroids/systems/spatialcagesystem.h>
#include <asteroids/systems/shootsystem.h>
#include <asteroids/systems/rockssystem.h>
#include <asteroids/systems/outofboundsystem.h>
#include <asteroids/systems/impactsystem.h>
#include <asteroids/fakedata/data.h>

#include <asteroids/flow/layers/base/layermanager.h>

#include <engine/ecs/components/icameracomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/systems/irendersystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/services/iengineapplicationservice.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>

#include <asteroids/flow/layers/commonlayer.h>

using namespace puma;

namespace ast
{
    namespace
    {
        pina::Entity spawnBackground()
        {
            pina::Entity result = gEntities->requestEntity();

            gComponents->add<ILocationComponent>( result );
            auto renderComponent = gComponents->add<IRenderComponent>( result );

            TextureInfo textureInfo;
            textureInfo.renderLayer = gData->kRenderLayers.Background;
            textureInfo.renderSize = { 1920.0f,1080.0f };
            textureInfo.texture = gData->kResourcesHandles.BackgroundTexture;

            renderComponent->addTextureInfo( textureInfo );

            return result;
        }

        void unspawnBackground( pina::Entity _entity )
        {
            gComponents->remove<ILocationComponent>( _entity );
            gComponents->remove<IRenderComponent>( _entity );
            gEntities->disposeEntity( _entity );
        }
    }

    void Asteroids::init()
    {
        m_data = std::make_unique<ast::Data>();
        puma::DefaultInstance<ast::Data>::setInstance( m_data.get() );
        puma::DefaultServices::getInstance()->registerClass<LayerService>();
        puma::DefaultServices::getInstance()->add<LayerService>();

        gEngineApplication->setWindowTitle( "Asteroids" );
        gEngineApplication->setWindowSize( 1920, 1080 );
        gEngineApplication->setWindowPosition( 200, 200 );

        auto sysProvider = gSystems;
        auto compProvider = gComponents;

        //Register classes
        sysProvider->registerSystemClass<ShipMovementSystem>();
        sysProvider->registerSystemClass<SpatialCageSystem>();
        sysProvider->registerSystemClass<ShootSystem>();
        sysProvider->registerSystemClass<RocksSystem>();
        sysProvider->registerSystemClass<OutOfBoundSystem>();
        sysProvider->registerSystemClass<ImpactSystem>();
        compProvider->registerClass<ShipComponent>();
        compProvider->registerClass<ShootComponent>();
        compProvider->registerClass<ImpactComponent>();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Load all resources now. TODO: Need to improve this once I implement a resource manager that does not clash with the renderer being used in the render loop
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        gData->kResourcesHandles.BackgroundTexture = gEngineApplication->getTextureManager()->loadTexture( gData->kResourcesPaths.BackgroundTexture );
        gData->kResourcesHandles.ShipTexture = gEngineApplication->getTextureManager()->loadTexture( gData->kResourcesPaths.ShipSprite );
        gData->kResourcesHandles.RockTexture = gEngineApplication->getTextureManager()->loadTexture( gData->kResourcesPaths.RockTexture );
        gData->kResourcesHandles.Font = gEngineApplication->getTextureManager()->loadFont( gData->kResourcesPaths.FontPath );

        nina::TexturizedTextInfo textInfo;
        textInfo.color = Color::White();
        textInfo.fontPath = gData->kResourcesPaths.FontPath;
        textInfo.text = "Press A to play";

        gData->kResourcesHandles.PressAnyButtonTexture = gEngineApplication->getTextureManager()->loadText( textInfo );
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        gLayerManager->addLayer( std::make_unique<CommonLayer>() );
        
        m_stateMachineInfo.currentStateId = gData->kGameStates.MenuState;

        m_stateMachine.addState<MenuState>();
        m_stateMachine.addState<GameplayState>();
        m_stateMachine.start( m_stateMachineInfo );

        m_backgroundEntity = spawnBackground();
    }

    void Asteroids::uninit()
    {
        unspawnBackground( m_backgroundEntity );
        
        m_stateMachine.uninit( m_stateMachineInfo );

        gLayerManager->removeLayer( gData->kGameLayers.CommonLayer );


        gEngineApplication->getTextureManager()->unloadFont( gData->kResourcesHandles.Font );
        gEngineApplication->getTextureManager()->unloadText( gData->kResourcesHandles.PressAnyButtonTexture );
        gEngineApplication->getTextureManager()->unloadTexture( gData->kResourcesHandles.BackgroundTexture );
        gEngineApplication->getTextureManager()->unloadTexture( gData->kResourcesHandles.ShipTexture );
        gEngineApplication->getTextureManager()->unloadTexture( gData->kResourcesHandles.RockTexture );

    }

    void Asteroids::update( float _deltaTime )
    {
        m_stateMachine.update( m_stateMachineInfo );
    }



}
