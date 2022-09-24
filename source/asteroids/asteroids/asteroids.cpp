#include <precompiledengine.h>
#include "asteroids.h"


#include <asteroids/components/shipcomponent.h>
#include <asteroids/components/shootcomponent.h>
#include <asteroids/fakedata/spawners/shipspawner.h>
#include <asteroids/fakedata/data.h>
#include <asteroids/systems/shipmovementsystem.h>
#include <asteroids/systems/spatialcagesystem.h>
#include <asteroids/systems/shootsystem.h>

#include <engine/ecs/components/icameracomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/systems/irendersystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/services/iengineapplicationservice.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>

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
            textureInfo.texture = gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.BackgroundTexture );

            renderComponent->addTextureInfo( textureInfo );

            gSystems->getSystem<IRenderSystem>()->registerEntity( result );

            return result;
        }

        void unspawnBackground( Entity _entity )
        {
            gComponents->removeComponent<IRenderComponent>( _entity );
            gSystems->getSystem<IRenderSystem>()->unregisterEntity( _entity );
            gEntities->disposeEntity( _entity );
        }
    }

    void Asteroids::init()
    {
        m_data = std::make_unique<ast::Data>();
        puma::DefaultInstance<ast::Data>::setInstance( m_data.get() );

        gEngineApplication->setWindowTitle( "Asteroids" );
        gEngineApplication->setWindowSize( 1920, 1080 );
        gEngineApplication->setWindowPosition( 200, 200 );

        auto sysProvider = gSystems;

        //Register classes
        sysProvider->registerSystem<ShipMovementSystem>();
        sysProvider->registerSystem<SpatialCageSystem>();
        sysProvider->registerSystem<ShootSystem>();
        gComponents->registerComponent<ShipComponent>();
        gComponents->registerComponent<ShootComponent>();

        sysProvider->addSystem<ICollisionSystem>();
        sysProvider->addSystem<IRenderSystem>();
        sysProvider->addSystem<IInputSystem>();

        //Inits
        initCamera();
        initPhysics();

        sysProvider->addSystem<ShipMovementSystem>();
        sysProvider->addSystem<SpatialCageSystem>();
        sysProvider->addSystem<ShootSystem>();

        gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.BackgroundTexture );
        gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.ShipSprite );

        //Spawn
        m_shipEntity = ShipSpawner::spawnShip( Position() );

        m_backgroundEntity = spawnBackground();
    }

    void Asteroids::uninit()
    {
        unspawnBackground( m_backgroundEntity );
        uninitCamera();
        ShipSpawner::unspawnShip( m_shipEntity );

        auto sysProvider = gSystems;

        sysProvider->removeSystem<ShipMovementSystem>();
        sysProvider->removeSystem<SpatialCageSystem>();
        sysProvider->removeSystem<ShootSystem>();

        sysProvider->removeSystem<ICollisionSystem>();
        sysProvider->removeSystem<IRenderSystem>();
        sysProvider->removeSystem<IInputSystem>();
    }

    void Asteroids::update( float _deltaTime )
    {

    }

    void Asteroids::initPhysics()
    {
        ICollisionSystem* collisionSystem = gSystems->getSystem<ICollisionSystem>();
        collisionSystem->setGravity( { 0.0f, 0.0f } );
        //collisionSystem->enableDebugDraw();
        collisionSystem->setCollisionCompatibility( gData->kCollisionCompatibility );
    }

    void Asteroids::initCamera()
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        m_cameraEntity = entityProvider->requestEntity();

        auto cameraComponent = componentProvider->addComponent<ICameraComponent>( m_cameraEntity );
        auto locationComponent = componentProvider->addComponent<ILocationComponent>( m_cameraEntity );

        cameraComponent->setMetersPerPixel( 1.0f );
        gEngineApplication->setCameraEntity( m_cameraEntity );
        locationComponent->setPosition( Position() );

    }

    void Asteroids::uninitCamera()
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        componentProvider->removeComponent<ICameraComponent>( m_cameraEntity );
        componentProvider->removeComponent<ILocationComponent>( m_cameraEntity );

        entityProvider->disposeEntity( m_cameraEntity );
    }
}
