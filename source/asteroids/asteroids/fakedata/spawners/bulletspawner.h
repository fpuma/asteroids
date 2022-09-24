#pragma once

#include <modules/pina/entity.h>
#include <engine/utils/position.h>
#include <modules/nina/ninadefinitions.h>

using namespace puma;
namespace ast
{
    class BulletSpawner
    {
    public:

        static Entity spawnBullet( Position _pos );
        static void unspawnBullet( Entity _entity );

    };
}