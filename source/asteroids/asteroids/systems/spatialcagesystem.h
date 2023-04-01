#pragma once

#include <engine/ecs/systems/base/isystem.h>
#include <engine/services/ecsservice.h>
#include <engine/utils/position.h>
#include <pina/entity.h>

using namespace puma;

namespace ast
{
    class SpatialCageSystem : public ISystem
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider ) override;
        void onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint ) override;

    private:

        struct TeleportInfo
        {
            pina::Entity entity;
            Position position;
        }m_pendingTeleport;

        void teleportEntity( pina::Entity _entity, Position _pos );
        Position getEntityPosition( pina::Entity _entity ) const;
        pina::Entity getEntityFromFramePart( leo::FramePartID _framePartId ) const;
        bool isCage( leo::FramePartID _framePartId ) const;
        void setPendingTeleport( leo::FramePartID _cageFramePart, leo::FramePartID _entityFramePart );
        pina::Entity m_spatialCage;

        leo::FramePartID m_top;
        leo::FramePartID m_bot;
        leo::FramePartID m_left;
        leo::FramePartID m_right;
    };
}