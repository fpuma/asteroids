#pragma once

#include <modules/pina/entity.h>
#include <engine/utils/position.h>
#include <modules/nina/ninadefinitions.h>

using namespace puma;
namespace ast
{
    class RockSpawner
    {
    public:

        static Entity spawnRock();
        static void unspawnRock( Entity _entity );

    };
}