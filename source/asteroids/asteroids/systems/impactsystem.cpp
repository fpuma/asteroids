#include <precompiledengine.h>
#include "impactsystem.h"

#include <asteroids/components/impactcomponent.h>
#include <asteroids/fakedata/data.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>


namespace ast
{
    void ImpactSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<ImpactSystem>( SystemUpdateId::PostPhysics );
        gSystems->subscribeSystemUpdate<ImpactSystem>( SystemUpdateId::CollisionStarted );

    }

    void ImpactSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<ImpactSystem>( SystemUpdateId::PostPhysics );
        gSystems->unsubscribeSystemUpdate<ImpactSystem>( SystemUpdateId::CollisionStarted );
    }

    void ImpactSystem::postPhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider )
    {
        for (const Entity& ntt : m_pendingDisable)
        {
            _entityProvider.disableEntity( ntt );
        }

        m_pendingDisable.clear();
    }

    void ImpactSystem::onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint )
    {
        Entity entityA = getEntityFromFramePart( _framePartPtrA );

        ComponentProvider* compProvider = gComponents;

        if (compProvider->containsComponent<ImpactComponent>( entityA ))
        {
            Entity entityB = getEntityFromFramePart( _framePartPtrB );

            if (compProvider->containsComponent<ImpactComponent>( entityB ))
            {
                ImpactComponent* impactComponentA = compProvider->getComponent<ImpactComponent>( entityA );
                ImpactComponent* impactComponentB = compProvider->getComponent<ImpactComponent>( entityB );

                impactComponentA->setCurrentHp( impactComponentA->getCurrentHp() - impactComponentB->getDamage() );
                impactComponentB->setCurrentHp( impactComponentB->getCurrentHp() - impactComponentA->getDamage() );

                if (impactComponentA->getCurrentHp() < 1)
                {
                    m_pendingDisable.insert( entityA );
                }

                if (impactComponentB->getCurrentHp() < 1)
                {
                    m_pendingDisable.insert( entityB );
                }
            }
        }
    }

    Entity ImpactSystem::getEntityFromFramePart( leo::FramePartID _framePartId ) const
    {
        leo::UserCollisionData userData = gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartId );
        return Entity( (size_t)userData );
    }

}