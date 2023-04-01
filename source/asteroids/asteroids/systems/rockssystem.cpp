#include <precompiledengine.h>
#include "rockssystem.h"

#include <asteroids/fakedata/spawners/rockspawner.h>
#include <asteroids/fakedata/data.h>
#include <asteroids/components/impactcomponent.h>
#include <asteroids/components/shootcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
#include <engine/ecs/components/iinputcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/renderer/irenderqueue.h>
#include <engine/services/iloggerservice.h>
#include <utils/geometry/geometryhelpers.h>

namespace ast
{

    namespace
    {
        Vec2 randomPoint( const Rectangle& _rect, puma::Random& _random )
        {
            Vec2 result;
            unsigned int min = static_cast<unsigned int>(_rect.lowerBoundary.x * 1000.0f);
            unsigned int max = static_cast<unsigned int>(_rect.upperBoundary.x * 1000.0f);
            result.x = static_cast<float>(_random.generateRandom(min, max)) / 1000.0f;

            min = static_cast<unsigned int>(_rect.lowerBoundary.y * 1000.0f);
            max = static_cast<unsigned int>(_rect.upperBoundary.y * 1000.0f);
            result.y = static_cast<float>(_random.generateRandom( min, max )) / 1000.0f;

            return result;
        }
    }

    void RocksSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<RocksSystem>( SystemUpdateId::PrePhysics );
        gSystems->subscribeSystemUpdate<RocksSystem>( SystemUpdateId::QueueRenderables );

        for (pina::Entity& rock : m_rocks)
        {
            rock = RockSpawner::spawnRock();
            gEntities->disableEntity( rock );
        }

        m_spawnCooldown.setCooldownTime( 1.0f / gData->kRockInfo.spawnRate );

        Vec2 upperCorner =
        {
            (gData->kSpatialCageInfo.width / 2.0f) + gData->kRockInfo.radius,
            (gData->kSpatialCageInfo.height / 2.0f) + (gData->kRockInfo.radius)
        };

        Vec2 lowerCorner =
        {
            (-gData->kSpatialCageInfo.width / 2.0f) - gData->kRockInfo.radius,
            (-gData->kSpatialCageInfo.height / 2.0f) - gData->kRockInfo.radius
        };



        m_rockPoints[0] = { upperCorner, {0.0f, upperCorner.y} };
        m_rockPoints[1] = { {0.0f, upperCorner.y}, { lowerCorner.x, upperCorner.y } };
        m_rockPoints[2] = { { lowerCorner.x, upperCorner.y }, {lowerCorner.x, 0.0f} };
        m_rockPoints[3] = { { lowerCorner.x, 0.0f }, lowerCorner };
        m_rockPoints[4] = { { 0.0f, lowerCorner.y }, lowerCorner };
        m_rockPoints[5] = { { upperCorner.x, lowerCorner.y }, { 0.0f, lowerCorner.y } };
        m_rockPoints[6] = { { upperCorner.x, 0.0f}, { upperCorner.x, lowerCorner.y } };
        m_rockPoints[7] = { upperCorner, {upperCorner.x, 0.0f} };

        m_random.refreshSeed( 888 );

    }

    void RocksSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<RocksSystem>( SystemUpdateId::PrePhysics );
        gSystems->unsubscribeSystemUpdate<RocksSystem>( SystemUpdateId::QueueRenderables );

        for (pina::Entity& rock : m_rocks)
        {
            RockSpawner::unspawnRock( rock );
        }
    }

    void RocksSystem::prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider )
    {

        if (m_spawnCooldown.cooldownReady())
        {
            auto itRock = std::find_if( m_rocks.begin(), m_rocks.end(), [&_entityProvider]( const pina::Entity& ntt )
                {
                    return !_entityProvider.isEntityEnabled( ntt );
                } );

            assert( itRock != m_rocks.end() ); // Rock pool is too small
            if (itRock != m_rocks.end())
            {
                pina::Entity rockEntity = *itRock;
                _entityProvider.enableEntity( rockEntity );
                ICollisionComponent* collisionComponent = _componentProvider.get<ICollisionComponent>( rockEntity );

                u32 index = m_random.generateRandom( 0, 7 );
                Position origin = randomPoint( m_rockPoints[index], m_random );

                index += 3;
                index = index > 7 ? (index % 7) - 1 : index;
                Position destination = randomPoint( m_rockPoints[index], m_random );

                leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();
                frame->setPosition( origin );
                Vec2 dir = destination - origin;
                dir = dir.normalize();
                frame->setLinearVelocity( dir * 100.0f );

                ImpactComponent* impactComponent = gComponents->get<ImpactComponent>( rockEntity );
                impactComponent->setCurrentHp( 10 );
            }
        }
     
    }

    void RocksSystem::queueRenderables( IRenderQueue& _renderQueue )
    {
        for (const pina::Entity& ntt : m_rocks)
        {
            if (gEntities->isEntityEnabled( ntt ))
            {
                ImpactComponent* impactComponent = gComponents->get<ImpactComponent>( ntt );
                ILocationComponent* locationComponent = gComponents->get<ILocationComponent>( ntt );
                _renderQueue.addDebugRenderableText( formatString( "HP: %d", impactComponent->getCurrentHp() ), Color::White(), locationComponent->getPosition() );
            }
        }
    }
}