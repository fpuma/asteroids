#include <precompiledengine.h>
#include "bulletspawner.h"

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
    pina::Entity BulletSpawner::spawnBullet( Position _pos )
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        pina::Entity bulletEntity = entityProvider->requestEntity();

        //Location
        componentProvider->add<ILocationComponent>( bulletEntity );

        //Collision
        auto collisionComponent = componentProvider->add<ICollisionComponent>( bulletEntity );

        leo::FrameInfo frameInfo;

        collisionComponent->init( leo::FrameType::Dynamic, frameInfo );

        leo::TriggerInfo triggerInfo;
        Circle circle = { Vec2(), gData->kBulletInfo.radius };
        triggerInfo.shape.setAsCircle( circle );
        
        triggerInfo.collisionIndex = gData->kCollisionIndexes.ShipSkill;
        triggerInfo.userData = (void*)bulletEntity.value();
        collisionComponent->addTrigger( triggerInfo );

        //Render
        auto renderComponent = componentProvider->add<IRenderComponent>( bulletEntity );

        TextureInfo textureInfo;
        textureInfo.texture = gData->kResourcesHandles.ShipTexture;
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.renderSize = { gData->kBulletInfo.radius*2, gData->kBulletInfo.radius*2 };
        //textureInfo.textureSample = { {0.6883f, 0.6048f}, {0.355f, 0.366f} };
        textureInfo.textureSample = { {1.0f, 1.0f}, {0.666f, 0.666f} };

        renderComponent->addTextureInfo( textureInfo );

        componentProvider->add<ImpactComponent>( bulletEntity );

        return bulletEntity;
    }
    
    void BulletSpawner::unspawnBullet( pina::Entity _entity )
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