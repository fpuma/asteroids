#include <precompiledengine.h>
#include "bulletspawner.h"

#include <asteroids/fakedata/data.h>

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
    Entity BulletSpawner::spawnBullet( Position _pos )
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        Entity bulletEntity = entityProvider->requestEntity();

        //Location
        componentProvider->addComponent<ILocationComponent>( bulletEntity );

        //Collision
        auto collisionComponent = componentProvider->addComponent<ICollisionComponent>( bulletEntity );

        leo::FrameInfo frameInfo;
        gSystems->getSystem<ICollisionSystem>()->registerEntity( bulletEntity, frameInfo, leo::FrameType::Kinematic );

        leo::BodyInfo bodyInfo;
        Circle circle = { Vec2(), gData->kBulletInfo.radius };
        bodyInfo.shape.setAsCircle( circle );
        
        bodyInfo.collisionIndex = gData->kCollisionIndexes.ShipSkill;
        bodyInfo.userData = (void*)bulletEntity.value();
        collisionComponent->addBody( bodyInfo );

        //Render
        auto renderComponent = componentProvider->addComponent<IRenderComponent>( bulletEntity );

        TextureInfo textureInfo;
        textureInfo.texture = gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.ShipSprite );
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.renderSize = { gData->kBulletInfo.radius*2, gData->kBulletInfo.radius*2 };
        //textureInfo.textureSample = { {0.6883f, 0.6048f}, {0.355f, 0.366f} };
        textureInfo.textureSample = { {1.0f, 1.0f}, {0.666f, 0.666f} };

        renderComponent->addTextureInfo( textureInfo );
        gSystems->getSystem<IRenderSystem>()->registerEntity( bulletEntity );

        return bulletEntity;
    }

    void BulletSpawner::unspawnBullet( Entity _entity )
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        gSystems->getSystem<ICollisionSystem>()->unregisterEntity( _entity );
        gSystems->getSystem<IRenderSystem>()->unregisterEntity( _entity );

        componentProvider->removeComponent<ILocationComponent>( _entity );
        componentProvider->removeComponent<ICollisionComponent>( _entity );


        entityProvider->disposeEntity( _entity );
    }
}