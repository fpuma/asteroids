#pragma once

#include <engine/ecs/systems/base/isystem.h>
#include <pina/entity.h>

#include <array>
#include <set>

using namespace puma;

namespace ast
{
    class ShootSystem : public puma::ISystem
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider ) override;

    private:

        std::array<pina::Entity, 100> m_bullets;

    };
}