#include <precompiledengine.h>
#include "shootsystem.h"

#include <asteroids/fakedata/spawners/bulletspawner.h>
#include <asteroids/fakedata/data.h>
#include <asteroids/components/shootcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
#include <engine/ecs/components/iinputcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/services/iloggerservice.h>
#include <utils/geometry/geometryhelpers.h>

namespace ast
{

    void ShootSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<ShootSystem>( SystemUpdateId::PrePhysics );

        for (pina::Entity& bullet : m_bullets)
        {
            bullet = BulletSpawner::spawnBullet( {} );
            gEntities->disableEntity( bullet );
        }

    }

    void ShootSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<ShootSystem>( SystemUpdateId::PrePhysics );

        for (pina::Entity& bullet : m_bullets)
        {
            BulletSpawner::unspawnBullet( bullet );
        }
    }

    void ShootSystem::prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider )
    {
        auto entities = gECS->getEntitesByComponents<ShootComponent, IInputComponent, ILocationComponent>();

        for (const pina::Entity& shooter : entities)
        {
            if (!_entityProvider.isEntityEnabled( shooter )) continue;

            //Input
            auto inputComponent = _componentProvider.get<IInputComponent>( shooter );
            auto shootComponent = _componentProvider.get<ShootComponent>( shooter );

            if (inputComponent->isActionActive( gData->kInputActions.Shoot ))
            {
                //gLogger->info("Engaged");
                shootComponent->engage();
            }

            if (inputComponent->isActionActive( gData->kInputActions.StopShoot ))
            {
                //gLogger->info( "Disengaged" );
                shootComponent->disengage();
            }

            if (shootComponent->isEngaged())
            {
                if (shootComponent->tryShoot())
                {
                    //gLogger->info( "Shotfired" );
                    auto itBullet = std::find_if( m_bullets.begin(), m_bullets.end(), [&_entityProvider]( const pina::Entity& bullet )
                        {
                            return !_entityProvider.isEntityEnabled( bullet );
                        });

                    assert( itBullet != m_bullets.end() ); // Need bigger bullet pool

                    pina::Entity currentBullet = *itBullet;
                    _entityProvider.enableEntity( currentBullet );
                    ICollisionComponent* collisionComponent = _componentProvider.get<ICollisionComponent>(currentBullet);
                    ILocationComponent* locationComponent = _componentProvider.get<ILocationComponent>( shooter );

                    leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();
                    Vec2 velocity = GeometryHelpers::rotatePoint2D( { 1.0f, 0 }, locationComponent->getRadiansRotation() ) * shootComponent->getBulletSpeed();
                    frame->setLinearVelocity( velocity );
                    frame->setPosition( { locationComponent->getPosition().x, locationComponent->getPosition().y } );
                }
            }
        }
    }
}