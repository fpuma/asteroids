#pragma once

#include <engine/services/ecsservice.h>
#include <modules/pina/entity.h>
#include <engine/utils/position.h>

using namespace puma;

namespace ast
{
    class SpatialCageSystem : public System
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;
        void onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint ) override;

    private:

        struct TeleportInfo
        {
            Entity entity;
            Position position;
        }m_pendingTeleport;

        void teleportEntity( Entity _entity, Position _pos );
        Position getEntityPosition( Entity _entity ) const;
        Entity getEntityFromFramePart( leo::FramePartID _framePartId ) const;
        bool isCage( leo::FramePartID _framePartId ) const;
        void setPendingTeleport( leo::FramePartID _cageFramePart, leo::FramePartID _entityFramePart );
        Entity m_spatialCage;

        leo::FramePartID m_top;
        leo::FramePartID m_bot;
        leo::FramePartID m_left;
        leo::FramePartID m_right;
    };
}