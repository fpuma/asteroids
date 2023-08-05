#pragma once

#include <pina/entity.h>
#include <engine/ecs/systems/base/isystem.h>

using namespace puma;

namespace ast
{
    class ShipMovementSystem : public ISystem
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider ) override;
        void queueRenderables( IRenderQueue& _renderQueue ) override;

    private:

        float processCurrentAngle( float _rawAngle );

    };
}
