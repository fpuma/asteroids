#pragma once

#include <pina/entity.h>
#include <engine/utils/position.h>
#include <modules/nina/ninadefinitions.h>

using namespace puma;
namespace ast
{
    class RockSpawner
    {
    public:

        static pina::Entity spawnRock();
        static void unspawnRock( pina::Entity _entity );

    };
}