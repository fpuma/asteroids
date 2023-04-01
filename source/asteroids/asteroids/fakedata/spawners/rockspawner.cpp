#include <precompiledengine.h>
#include "rockspawner.h"

#include <asteroids/fakedata/data.h>
#include <asteroids/components/impactcomponent.h>

#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>
#include <engine/ecs/systems/irendersystem.h>
#include <utils/geometry/geometryhelpers.h>

#include <engine/services/iengineapplicationservice.h>

namespace ast
{
    pina::Entity RockSpawner::spawnRock( )
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        pina::Entity rockEntity = entityProvider->requestEntity();

        //Location
        componentProvider->add<ILocationComponent>( rockEntity );

        //Collision
        auto collisionComponent = componentProvider->add<ICollisionComponent>( rockEntity );

        leo::FrameInfo frameInfo;

        collisionComponent->init( leo::FrameType::Dynamic, frameInfo);

        leo::BodyInfo bodyInfo;
        Circle circle = { Vec2(), gData->kRockInfo.radius };
        bodyInfo.shape.setAsCircle( circle );
        
        bodyInfo.collisionIndex = gData->kCollisionIndexes.Asteroid;
        bodyInfo.userData = (void*)rockEntity.value();
        collisionComponent->addBody( bodyInfo );

        //Render
        auto renderComponent = componentProvider->add<IRenderComponent>( rockEntity );

        TextureInfo textureInfo;
        textureInfo.texture = gData->kResourcesHandles.RockTexture;
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.renderSize = { gData->kRockInfo.radius*2, gData->kRockInfo.radius*2 };

        renderComponent->addTextureInfo( textureInfo );

        auto impactComponent = componentProvider->add<ImpactComponent>( rockEntity );
        impactComponent->setCurrentHp( 10 );

        return rockEntity;
    }

    void RockSpawner::unspawnRock( pina::Entity _entity )
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        componentProvider->remove<ILocationComponent>( _entity );
        componentProvider->remove<ICollisionComponent>( _entity );
        componentProvider->remove<IRenderComponent>( _entity );
        componentProvider->remove<ImpactComponent>( _entity );

        entityProvider->disposeEntity( _entity );
    }
}