#include <precompiledengine.h>
#include "asteroids.h"


#include <asteroids/components/shipcomponent.h>
#include <asteroids/fakedata/spawners/shipspawner.h>
#include <asteroids/fakedata/ids/ids.h>
#include <asteroids/systems/shipmovementsystem.h>

#include <engine/ecs/components/icameracomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/systems/irendersystem.h>
#include <engine/services/iengineapplicationservice.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>

using namespace puma;

namespace
{
    Entity spawnBackground()
    {
        Entity result = gEntities->requestEntity();

        gComponents->addComponent<ILocationComponent>( result );
        auto renderComponent = gComponents->addComponent<IRenderComponent>( result );

        TextureInfo textureInfo;
        textureInfo.renderLayer = RenderLayers::Background;
        textureInfo.renderSize = { 1000.0f,1000.0f };
        textureInfo.texture = gEngineApplication->getTextureManager()->loadTexture( "../assets/asteroids/backgroundSpace_01.1.png" );
        
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
    gEngineApplication->setWindowTitle( "Asteroids" );
    gEngineApplication->setWindowSize( 1000, 1000 );
    gEngineApplication->setWindowPosition( 200, 200 );

    //Register classes
    gSystems->registerSystem<ShipMovementSystem>();
    gSystems->addSystem<ShipMovementSystem>();
    gComponents->registerComponent<ShipComponent>();

    gSystems->subscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::PrePhysics );
    gSystems->subscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::QueueRenderables );

    //Inits
    initCamera();
    initPhysics();
    
    gEngineApplication->getTextureManager()->loadTexture( "../assets/asteroids/backgroundSpace_01.1.png" );
    nina::Texture shipTexture = gEngineApplication->getTextureManager()->loadTexture( "../assets/asteroids/FighterPlaneV2.png" );

    //Spawn
    m_shipEntity = ShipSpawner::spawnShip( shipTexture, Position() );

    gSystems->getSystem<ICollisionSystem>()->disableDebugDraw();

    m_backgroundEntity = spawnBackground();
}

void Asteroids::uninit()
{
    unspawnBackground( m_backgroundEntity );
    uninitCamera();
    ShipSpawner::unspawnShip( m_shipEntity );

    gSystems->unsubscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::PrePhysics );
    gSystems->unsubscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::QueueRenderables );
    gSystems->removeSystem<ShipMovementSystem>();
}

void Asteroids::update( float _deltaTime )
{

}

void Asteroids::initPhysics()
{
    ICollisionSystem* collisionSystem = gSystems->getSystem<ICollisionSystem>();
    collisionSystem->init( { 0.0f, 0.0f } );
    collisionSystem->enableDebugDraw();
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
