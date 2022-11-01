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

        sysProvider->addSystem<ShipMovementSystem>();
        sysProvider->addSystem<SpatialCageSystem>();
        sysProvider->addSystem<ShootSystem>();
        sysProvider->addSystem<RocksSystem>();
        sysProvider->addSystem<OutOfBoundSystem>();
        sysProvider->addSystem<ImpactSystem>();

        m_shipEntity = ShipSpawner::spawnShip( Position() );
    }

    void GameplayLayer::onUninit()
    {
        ShipSpawner::unspawnShip( m_shipEntity );

        auto sysProvider = gSystems;

        sysProvider->removeSystem<ShipMovementSystem>();
        sysProvider->removeSystem<SpatialCageSystem>();
        sysProvider->removeSystem<ShootSystem>();
        sysProvider->removeSystem<RocksSystem>();
        sysProvider->removeSystem<OutOfBoundSystem>();
        sysProvider->removeSystem<ImpactSystem>();

        sysProvider->removeSystem<ICollisionSystem>();

    }

    void GameplayLayer::initPhysics()
    {
        auto collisionSystem = gSystems->addSystem<ICollisionSystem>();
        collisionSystem->setGravity( { 0.0f, 0.0f } );
        //collisionSystem->enableDebugDraw();
        collisionSystem->setCollisionCompatibility( gData->kCollisionCompatibility );
    }
}