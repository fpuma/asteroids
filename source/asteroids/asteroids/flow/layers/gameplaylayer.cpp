#include <precompiledengine.h>

#include "gameplaylayer.h"

#include <asteroids/fakedata/data.h>
#include <asteroids/fakedata/spawners/shipspawner.h>
#include <asteroids/systems/shipmovementsystem.h>
#include <asteroids/systems/spatialcagesystem.h>
#include <asteroids/systems/shootsystem.h>
#include <asteroids/systems/rockssystem.h>
#include <asteroids/systems/outofboundsystem.h>
#include <asteroids/systems/impactsystem.h>


#include <engine/ecs/systems/icollisionsystem.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>

namespace ast
{
    
    GameplayLayer::GameplayLayer()
        : ILayer(gData->kGameLayers.GameplayLayer, gData->kGameLayers.CommonLayer)
    {

    }

    void GameplayLayer::onInit()
    {
        auto sysProvider = gSystems;

        initPhysics();

        sysProvider->requestSystem<ShipMovementSystem>();
        sysProvider->requestSystem<SpatialCageSystem>();
        sysProvider->requestSystem<ShootSystem>();
        sysProvider->requestSystem<RocksSystem>();
        sysProvider->requestSystem<OutOfBoundSystem>();
        sysProvider->requestSystem<ImpactSystem>();

        m_shipEntity = ShipSpawner::spawnShip( Position() );
    }

    void GameplayLayer::onUninit()
    {
        ShipSpawner::unspawnShip( m_shipEntity );

        auto sysProvider = gSystems;

        sysProvider->releaseSystem<ShipMovementSystem>();
        sysProvider->releaseSystem<SpatialCageSystem>();
        sysProvider->releaseSystem<ShootSystem>();
        sysProvider->releaseSystem<RocksSystem>();
        sysProvider->releaseSystem<OutOfBoundSystem>();
        sysProvider->releaseSystem<ImpactSystem>();

        sysProvider->releaseSystem<ICollisionSystem>();

    }

    void GameplayLayer::initPhysics()
    {
        gSystems->requestSystem<ICollisionSystem>();
        auto collisionSystem = gSystems->getSystem<ICollisionSystem>();
        collisionSystem->setGravity( { 0.0f, 0.0f } );
        //collisionSystem->enableDebugDraw();
        collisionSystem->setCollisionCompatibility( gData->kCollisionCompatibility );
    }
}