#include <precompiledengine.h>
#include "asteroids.h"


#include <asteroids/components/shipcomponent.h>
#include <asteroids/components/shootcomponent.h>
#include <asteroids/components/impactcomponent.h>
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
#include <engine/ecs/systems/icollisionsystem.h>

#include <asteroids/flow/layers/commonlayer.h>
#include <asteroids/flow/layers/gameplaylayer.h>

using namespace puma;

namespace ast
{
    namespace
    {
        Entity spawnBackground()
        {
            Entity result = gEntities->requestEntity();

            gComponents->addComponent<ILocationComponent>( result );
            auto renderComponent = gComponents->addComponent<IRenderComponent>( result );

            TextureInfo textureInfo;
            textureInfo.renderLayer = gData->kRenderLayers.Background;
            textureInfo.renderSize = { 1920.0f,1080.0f };
            textureInfo.texture = gData->kTextureHandles.BackgroundTexture;

            renderComponent->addTextureInfo( textureInfo );

            gSystems->getSystem<IRenderSystem>()->registerEntity( result );

            return result;
        }

        void unspawnBackground( Entity _entity )
        {
            gComponents->removeComponent<ILocationComponent>( _entity );
            gComponents->removeComponent<IRenderComponent>( _entity );
            gSystems->getSystem<IRenderSystem>()->unregisterEntity( _entity );
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
        sysProvider->registerSystem<ShipMovementSystem>();
        sysProvider->registerSystem<SpatialCageSystem>();
        sysProvider->registerSystem<ShootSystem>();
        sysProvider->registerSystem<RocksSystem>();
        sysProvider->registerSystem<OutOfBoundSystem>();
        sysProvider->registerSystem<ImpactSystem>();
        compProvider->registerComponent<ShipComponent>();
        compProvider->registerComponent<ShootComponent>();
        compProvider->registerComponent<ImpactComponent>();

        gData->kTextureHandles.BackgroundTexture = gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.BackgroundTexture );
        gData->kTextureHandles.ShipTexture = gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.ShipSprite );
        gData->kTextureHandles.RockTexture = gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.RockTexture );
        
        gLayerManager->addLayer( std::make_unique<CommonLayer>() );
        gLayerManager->addLayer( std::make_unique<GameplayLayer>() );
        
        m_backgroundEntity = spawnBackground();
    }

    void Asteroids::uninit()
    {
        unspawnBackground( m_backgroundEntity );
        
        gLayerManager->removeLayer( gData->kGameLayers.CommonLayer );
    }

    void Asteroids::update( float _deltaTime )
    {

    }



}
