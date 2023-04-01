#pragma once

#include <pina/entity.h>
#include <engine/utils/position.h>
#include <modules/nina/ninadefinitions.h>

using namespace puma;
namespace ast
{
    class BulletSpawner
    {
    public:

        static pina::Entity spawnBullet( Position _pos );
        static void unspawnBullet( pina::Entity _entity );

    };
}