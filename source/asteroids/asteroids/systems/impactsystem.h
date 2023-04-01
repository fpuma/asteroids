#pragma once

#include <engine/ecs/systems/base/isystem.h>
#include <pina/entity.h>
#include <engine/utils/position.h>

using namespace puma;

namespace ast
{
    class ImpactSystem : public ISystem
    {
    public:

        void onInit() override;
        void onUninit() override;

        void postPhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider ) override;
        void onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint ) override;

    private:

        std::set<pina::Entity> m_pendingDisable;

        pina::Entity getEntityFromFramePart( leo::FramePartID _framePartId ) const;
    };
}