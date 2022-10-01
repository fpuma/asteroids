#include <precompiledengine.h>
#include "rockssystem.h"

#include <asteroids/fakedata/spawners/rockspawner.h>
#include <asteroids/fakedata/data.h>
#include <asteroids/components/shootcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/components/iinputcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/services/iloggerservice.h>
#include <utils/geometry/geometryhelpers.h>

namespace ast
{

    void RocksSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<RocksSystem>( SystemUpdateId::PrePhysics );

        for (Entity& rock : m_rocks)
        {
            rock = RockSpawner::spawnRock();
            gEntities->disableEntity( rock );
        }

        m_spawnCooldown.setCooldownTime( 1.0f / gData->kRockInfo.spawnRate );

    }

    void RocksSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<RocksSystem>( SystemUpdateId::PrePhysics );

        for (Entity& rock : m_rocks)
        {
            RockSpawner::unspawnRock( rock );
        }
    }

    void RocksSystem::prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider )
    {

        if (m_spawnCooldown.cooldownReady())
        {
            auto itRock = std::find_if( m_rocks.begin(), m_rocks.end(), [&_entityProvider]( const Entity& ntt )
                {
                    return !_entityProvider.isEntityEnabled( ntt );
                } );

            assert( itRock != m_rocks.end() ); // Rock pool is too small
            if (itRock != m_rocks.end())
            {
                Entity rockEntity = *itRock;
                _entityProvider.enableEntity( rockEntity );
                ICollisionComponent* collisionComponent = _componentProvider.getComponent<ICollisionComponent>( rockEntity );

                leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();
                frame->setPosition( {} );
                frame->setLinearVelocity( { 25.0f, 0.0f } );
            }
        }
     
    }
}