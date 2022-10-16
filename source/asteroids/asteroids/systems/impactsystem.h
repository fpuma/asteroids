#pragma once

#include <modules/pina/system.h>
#include <modules/pina/entity.h>
#include <engine/utils/position.h>

using namespace puma;

namespace ast
{
    class ImpactSystem : public System
    {
    public:

        void onInit() override;
        void onUninit() override;

        void postPhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;
        void onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint ) override;

    private:

        std::set<Entity> m_pendingDisable;

        Entity getEntityFromFramePart( leo::FramePartID _framePartId ) const;
    };
}