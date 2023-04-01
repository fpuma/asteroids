#pragma once

#include <engine/ecs/systems/base/isystem.h>
#include <pina/entity.h>
#include <engine/utils/cooldown.h>
#include <array>
#include <utils/random.h>

using namespace puma;

namespace ast
{
    class RocksSystem : public puma::ISystem
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider ) override;
        void queueRenderables( IRenderQueue& _renderQueue ) override;

    private:

        std::array<pina::Entity, 100> m_rocks;
        Cooldown m_spawnCooldown;

        std::array<Rectangle, 8> m_rockPoints;

        Random m_random;
    };
}