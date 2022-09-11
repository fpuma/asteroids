#pragma once

#include <modules/pina/entity.h>
#include <modules/pina/system.h>

using namespace puma;

namespace ast
{
    class ShipMovementSystem : public System
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;
        void queueRenderables( IRenderQueue& _renderQueue ) override;
        void setShipEntity( Entity _entity ) { m_shipEntity = _entity; }

    private:

        float processCurrentAngle( float _rawAngle );

        Entity m_shipEntity;
    };
}
