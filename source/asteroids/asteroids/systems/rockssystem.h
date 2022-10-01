#pragma once

#include <modules/pina/system.h>
#include <modules/pina/entity.h>
#include <engine/utils/cooldown.h>
#include <array>
#include <set>

using namespace puma;

namespace ast
{
    class RocksSystem : public puma::System
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;


    private:

        std::array<Entity, 100> m_rocks;
        Cooldown m_spawnCooldown;
    };
}