#include <precompiledengine.h>
#include "outofboundsystem.h"

#include <asteroids/fakedata/data.h>

#include <engine/services/ecsservice.h>

#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>

#include <engine/ecs/systems/icollisionsystem.h>

namespace ast
{
    void OutOfBoundSystem::spawnPlayfield()
    {
        m_playfield = gEntities->requestEntity();

        ComponentProvider* compProvider = gComponents;
        SystemProvider* sysProvider = gSystems;

        compProvider->addComponent<ILocationComponent>( m_playfield );
        auto collisionComponent = compProvider->addComponent<ICollisionComponent>( m_playfield );

        leo::FrameInfo frameInfo;
        sysProvider->getSystem<ICollisionSystem>()->registerEntity( m_playfield, frameInfo, leo::FrameType::Static );

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
        gSystems->getSystem<ICollisionSystem>()->unregisterEntity( m_playfield );

        gComponents->removeComponent<ICollisionComponent>( m_playfield );
        gComponents->removeComponent<ILocationComponent>( m_playfield );

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
        unspawnPlayfield();
        m_playfield.invalidate();
        m_framePartId.invalidate();
        m_pendingDisable.clear();
    }

    void OutOfBoundSystem::postPhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider )
    {
        for (const Entity& ntt : m_pendingDisable)
        {
            gEntities->disableEntity( ntt );
        }

        m_pendingDisable.clear();
    }

    void OutOfBoundSystem::onCollisionStopped( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB )
    {
        if (_framePartPtrA == m_framePartId)
        {
            Entity ntt( (size_t)gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartPtrB ) );
            m_pendingDisable.insert( ntt );
        }
        else if (_framePartPtrB == m_framePartId)
        {
            Entity ntt( (size_t)gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartPtrA ) );
            m_pendingDisable.insert( ntt );
        }
    }
}