#include <precompiledengine.h>
#include "spatialcagesystem.h"

#include <asteroids/fakedata/data.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/systems/icollisionsystem.h>
#include <engine/services/iloggerservice.h>
#include <utils/formatstring.h>

namespace ast
{
    void SpatialCageSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<SpatialCageSystem>( SystemUpdateId::PrePhysics );
        gSystems->subscribeSystemUpdate<SpatialCageSystem>( SystemUpdateId::CollisionStarted );

        m_spatialCage = gEntities->requestEntity();

        auto collisionComponent = gComponents->addComponent<ICollisionComponent>( m_spatialCage );
        gComponents->addComponent<ILocationComponent>( m_spatialCage );

        gSystems->getSystem<ICollisionSystem>()->registerEntity( m_spatialCage, {}, leo::FrameType::Static );

        //*
        leo::TriggerInfo triggerInfo;
        triggerInfo.collisionIndex = gData->kCollisionIndexes.SpatialCage;
        triggerInfo.userData = (void*)(m_spatialCage.value());
        
        Rectangle rect = { { 800.0f, 400.0f},{-800.0f, 350.0f} };
        triggerInfo.shape.setAsPolygon( rect );
        m_top = collisionComponent->addTrigger( triggerInfo );

        rect = { { 800.0f, -350.0f},{-800.0f, -400.0f} };
        triggerInfo.shape.setAsPolygon( rect );
        m_bot = collisionComponent->addTrigger( triggerInfo );

        rect = { { -750.0f, 400.0f},{-800.0f, -400.0f} };
        triggerInfo.shape.setAsPolygon( rect );
        m_left = collisionComponent->addTrigger( triggerInfo );

        rect = { { 800.0f, 400.0f},{ 750.0f, -400.0f} };
        triggerInfo.shape.setAsPolygon( rect );
        m_right = collisionComponent->addTrigger( triggerInfo );

    }

    void SpatialCageSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<SpatialCageSystem>( SystemUpdateId::CollisionStarted );

        gSystems->getSystem<ICollisionSystem>()->unregisterEntity( m_spatialCage );

        gComponents->removeComponent<ICollisionComponent>( m_spatialCage );
        gComponents->removeComponent<ILocationComponent>( m_spatialCage );

        gEntities->disposeEntity( m_spatialCage );

    }

    void SpatialCageSystem::prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider )
    {
        if (m_pendingTeleport.entity.isValid())
        {
            teleportEntity( m_pendingTeleport.entity, m_pendingTeleport.position );
            m_pendingTeleport.entity.invalidate();
        }
    }

    void SpatialCageSystem::onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint )
    {
        if (isCage( _framePartPtrA ))
        {
            setPendingTeleport( _framePartPtrA, _framePartPtrB );
        }
        else if (isCage( _framePartPtrB ))
        {
            setPendingTeleport( _framePartPtrB, _framePartPtrA );
        }
    }

    void SpatialCageSystem::setPendingTeleport( leo::FramePartID _cageFramePart, leo::FramePartID _entityFramePart )
    {
        Entity entity = getEntityFromFramePart( _entityFramePart );
        Position pos = getEntityPosition( entity );
        if (_cageFramePart == m_top)
        {
            pos.y = -324.0f;
            m_pendingTeleport.position = pos;
        }

        if (_cageFramePart == m_bot)
        {
            pos.y = 324.0f;
            m_pendingTeleport.position = pos;
        }

        if (_cageFramePart == m_left)
        {
            pos.x = 724.0f;
            m_pendingTeleport.position = pos;
        }

        if (_cageFramePart == m_right)
        {
            pos.x = -724.0f;
            m_pendingTeleport.position = pos;
        }

        m_pendingTeleport.entity = entity;
    }

    void SpatialCageSystem::teleportEntity( Entity _entity, Position _pos )
    {
        ICollisionComponent* collisionComponent = gComponents->getComponent<ICollisionComponent>( _entity );

        leo::IDynamicFrame* dynFrame = collisionComponent->getDynamicFrame();
        dynFrame->setPosition( { _pos.x, _pos.y } );
    }

    Position SpatialCageSystem::getEntityPosition( Entity _entity ) const
    {
        ILocationComponent* locationComponent = gComponents->getComponent<ILocationComponent>( _entity );
        return locationComponent->getPosition();
    }

    Entity SpatialCageSystem::getEntityFromFramePart( leo::FramePartID _framePartId ) const
    {
        leo::UserCollisionData userData = gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartId );
        return Entity( (size_t)userData );
    }

    bool SpatialCageSystem::isCage( leo::FramePartID _framePartId ) const
    {
        return  (_framePartId == m_top) ||
                (_framePartId == m_bot) ||
                (_framePartId == m_left) ||
                (_framePartId == m_right);
    }
}