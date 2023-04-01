#pragma once

#include <engine/ecs/systems/base/isystem.h>
#include <pina/entity.h>
#include <modules/leo/leodefinitions.h>

#include <set>

using namespace puma;

namespace ast
{
    class OutOfBoundSystem : public ISystem
    {
    public:


        void onInit() override;
        void onUninit() override;

        void postPhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider ) override;
        void onCollisionStopped( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB ) override;

    private:

        void spawnPlayfield();
        void unspawnPlayfield();

        pina::Entity m_playfield;
        leo::FramePartID m_framePartId;
        std::set<pina::Entity> m_pendingDisable;
    };
}