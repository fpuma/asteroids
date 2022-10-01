#include <precompiledengine.h>
#include "rockspawner.h"

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
    Entity RockSpawner::spawnRock( )
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        Entity rockEntity = entityProvider->requestEntity();

        //Location
        componentProvider->addComponent<ILocationComponent>( rockEntity );

        //Collision
        auto collisionComponent = componentProvider->addComponent<ICollisionComponent>( rockEntity );

        leo::FrameInfo frameInfo;
        gSystems->getSystem<ICollisionSystem>()->registerEntity( rockEntity, frameInfo, leo::FrameType::Dynamic );

        leo::BodyInfo bodyInfo;
        Circle circle = { Vec2(), gData->kRockInfo.radius };
        bodyInfo.shape.setAsCircle( circle );
        
        bodyInfo.collisionIndex = gData->kCollisionIndexes.Asteroid;
        bodyInfo.userData = (void*)rockEntity.value();
        collisionComponent->addBody( bodyInfo );

        //Render
        auto renderComponent = componentProvider->addComponent<IRenderComponent>( rockEntity );

        TextureInfo textureInfo;
        textureInfo.texture = gEngineApplication->getTextureManager()->loadTexture( gData->kTexturePaths.RockTexture );
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.renderSize = { gData->kRockInfo.radius*2, gData->kRockInfo.radius*2 };

        renderComponent->addTextureInfo( textureInfo );
        gSystems->getSystem<IRenderSystem>()->registerEntity( rockEntity );

        return rockEntity;
    }

    void RockSpawner::unspawnRock( Entity _entity )
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        gSystems->getSystem<ICollisionSystem>()->unregisterEntity( _entity );
        gSystems->getSystem<IRenderSystem>()->unregisterEntity( _entity );

        componentProvider->removeComponent<ILocationComponent>( _entity );
        componentProvider->removeComponent<ICollisionComponent>( _entity );
        componentProvider->removeComponent<IRenderComponent>( _entity );

        entityProvider->disposeEntity( _entity );
    }
}