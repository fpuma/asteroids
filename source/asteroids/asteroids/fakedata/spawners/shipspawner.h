#pragma once

#include <modules/pina/entity.h>
#include <engine/utils/position.h>
#include <modules/nina/ninadefinitions.h>

using namespace puma;
namespace ast
{
    class ShipSpawner
    {
    public:

        static Entity spawnShip( nina::Texture _texture, Position _pos );
        static void unspawnShip( Entity _entity );

    };
}