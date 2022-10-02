#pragma once

#include <modules/pina/system.h>
#include <modules/pina/entity.h>
#include <modules/leo/leodefinitions.h>

#include <set>

using namespace puma;

namespace ast
{
    class OutOfBoundSystem : public System
    {
    public:


        void onInit() override;
        void onUninit() override;

        void postPhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;
        void onCollisionStopped( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB ) override;

    private:

        void spawnPlayfield();
        void unspawnPlayfield();

        Entity m_playfield;
        leo::FramePartID m_framePartId;
        std::set<Entity> m_pendingDisable;
    };
}