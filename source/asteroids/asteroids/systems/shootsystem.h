#pragma once

#include <modules/pina/system.h>
#include <modules/pina/entity.h>

#include <array>
#include <set>

using namespace puma;

namespace ast
{
    class ShootSystem : public puma::System
    {
    public:

        void onInit() override;
        void onUninit() override;

        void prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider ) override;

        void registerEntity( Entity _entity );
        void unregisterEntity( Entity _entity );

    private:

        std::set<Entity> m_entities;

        std::array<Entity, 100> m_bullets;

    };
}