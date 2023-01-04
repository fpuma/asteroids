#pragma once

#include <modules/pina/system.h>
#include <modules/pina/entity.h>
#include <engine/utils/cooldown.h>
#include <array>
#include <utils/random.h>

using namespace puma;

namespace ast
{
    class RocksSystem : public puma::System
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;
        void queueRenderables( IRenderQueue& _renderQueue ) override;

    private:

        std::array<Entity, 100> m_rocks;
        Cooldown m_spawnCooldown;

        std::array<Rectangle, 8> m_rockPoints;

        Random m_random;
    };
}