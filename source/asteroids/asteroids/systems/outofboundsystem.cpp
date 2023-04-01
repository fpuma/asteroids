#include <precompiledengine.h>
#include "outofboundsystem.h"

#include <asteroids/fakedata/data.h>

#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>

#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>

#include <engine/ecs/systems/icollisionsystem.h>

namespace ast
{
    void OutOfBoundSystem::spawnPlayfield()
    {
        m_playfield = gEntities->requestEntity();

        pina::ComponentProvider* compProvider = gComponents;

        compProvider->add<ILocationComponent>( m_playfield );
        auto collisionComponent = compProvider->add<ICollisionComponent>( m_playfield );

        leo::FrameInfo frameInfo;
        collisionComponent->init( leo::FrameType::Static, frameInfo );

        Vec2 upperCorner =
        {
            (gData->kSpatialCageInfo.width / 2.0f) + (gData->kSpatialCageInfo.threshold * 2),
            (gData->kSpatialCageInfo.height / 2.0f) + (gData->kSpatialCageInfo.threshold * 2)
        };

        Vec2 lowerCorner =
        {
            (-gData->kSpatialCageInfo.width / 2.0f) - (gData->kSpatialCageInfo.threshold * 2),
            (-gData->kSpatialCageInfo.height / 2.0f) - (gData->kSpatialCageInfo.threshold * 2)
        };

        leo::TriggerInfo triggerInfo;
        Rectangle rect;
        rect.lowerBoundary = lowerCorner;
        rect.upperBoundary = upperCorner;

        triggerInfo.shape.setAsPolygon( rect );
        triggerInfo.collisionIndex = gData->kCollisionIndexes.Playfield;
        triggerInfo.userData = (void*)m_playfield.value();

        m_framePartId = collisionComponent->addTrigger( triggerInfo );
    }

    void OutOfBoundSystem::unspawnPlayfield()
    {
        gComponents->remove<ICollisionComponent>( m_playfield );
        gComponents->remove<ILocationComponent>( m_playfield );

        gEntities->disposeEntity( m_playfield );
    }

    void OutOfBoundSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<OutOfBoundSystem>( SystemUpdateId::CollisionStopped);
        gSystems->subscribeSystemUpdate<OutOfBoundSystem>( SystemUpdateId::PostPhysics );
        spawnPlayfield();
    }

    void OutOfBoundSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<OutOfBoundSystem>( SystemUpdateId::CollisionStopped );
        gSystems->unsubscribeSystemUpdate<OutOfBoundSystem>( SystemUpdateId::PostPhysics );

        unspawnPlayfield();
        m_playfield.invalidate();
        m_framePartId.invalidate();
        m_pendingDisable.clear();
    }

    void OutOfBoundSystem::postPhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider )
    {
        for (const pina::Entity& ntt : m_pendingDisable)
        {
            if (gEntities->isEntityEnabled( ntt )) // [TODO] Check why this is disabling an already disabled entity
                gEntities->disableEntity( ntt );
        }

        m_pendingDisable.clear();
    }

    void OutOfBoundSystem::onCollisionStopped( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB )
    {
        if (_framePartPtrA == m_framePartId)
        {
            pina::Entity ntt( (size_t)gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartPtrB ) );
            m_pendingDisable.insert( ntt );
        }
        else if (_framePartPtrB == m_framePartId)
        {
            pina::Entity ntt( (size_t)gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartPtrA ) );
            m_pendingDisable.insert( ntt );
        }
    }
}