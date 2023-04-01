#include <precompiledengine.h>
#include "impactsystem.h"

#include <asteroids/components/impactcomponent.h>
#include <asteroids/fakedata/data.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
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

    void ImpactSystem::postPhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider )
    {
        for (const pina::Entity& ntt : m_pendingDisable)
        {
            _entityProvider.disableEntity( ntt );
        }

        m_pendingDisable.clear();
    }

    void ImpactSystem::onCollisionStarted( leo::FramePartID _framePartPtrA, leo::FramePartID _framePartPtrB, leo::ContactPoint _contactPoint )
    {
        pina::Entity entityA = getEntityFromFramePart( _framePartPtrA );

        pina::ComponentProvider* compProvider = gComponents;

        if (compProvider->contains<ImpactComponent>( entityA ))
        {
            pina::Entity entityB = getEntityFromFramePart( _framePartPtrB );

            if (compProvider->contains<ImpactComponent>( entityB ))
            {
                ImpactComponent* impactComponentA = compProvider->get<ImpactComponent>( entityA );
                ImpactComponent* impactComponentB = compProvider->get<ImpactComponent>( entityB );

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

    pina::Entity ImpactSystem::getEntityFromFramePart( leo::FramePartID _framePartId ) const
    {
        leo::UserCollisionData userData = gSystems->getSystem<ICollisionSystem>()->getUserCollisionData( _framePartId );
        return pina::Entity( (size_t)userData );
    }

}