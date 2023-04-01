#pragma once

#include <pina/entity.h>
#include <engine/utils/position.h>
#include <modules/nina/ninadefinitions.h>

using namespace puma;
namespace ast
{
    class ShipSpawner
    {
    public:

        static pina::Entity spawnShip( Position _pos );
        static void unspawnShip( pina::Entity _entity );

    };
}