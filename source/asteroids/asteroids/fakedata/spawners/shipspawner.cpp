#include <precompiledengine.h>
#include "shipspawner.h"

#include <asteroids/components/shipcomponent.h>
#include <asteroids/fakedata/input/inputconfig.h>
#include <asteroids/systems/shipmovementsystem.h>

#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/components/iinputcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/ecs/systems/irendersystem.h>
#include <utils/geometry/geometryhelpers.h>
#include <asteroids/fakedata/ids/ids.h>

Entity ShipSpawner::spawnShip( nina::Texture _texture, Position _pos )
{
    EntityProvider* entityProvider = gEntities;
    ComponentProvider* componentProvider = gComponents;

    Entity shipEntity = entityProvider->requestEntity();

    //Location
    componentProvider->addComponent<ILocationComponent>( shipEntity );

    //Input
    auto inputComponent = componentProvider->addComponent<IInputComponent>( shipEntity );
    for ( const auto& inputMap : kShipControllerJoystickInput )
    {
        inputComponent->addInputMap( inputMap.first, inputMap.second );
    }
    gSystems->getSystem<IInputSystem>()->registerEntity( shipEntity );

    //Collision
    auto collisionComponent = componentProvider->addComponent<ICollisionComponent>( shipEntity );

    leo::FrameInfo frameInfo;
    frameInfo.linearDamping = 0.1f;
    gSystems->getSystem<ICollisionSystem>()->registerEntity( shipEntity, frameInfo, leo::FrameType::Dynamic );

    leo::BodyInfo bodyInfo;
    Circle circle = { Vec2(), 25.0f };
    bodyInfo.shape.setAsCircle( circle );

    collisionComponent->addBody( bodyInfo );
    
    //Render
    auto renderComponent = componentProvider->addComponent<IRenderComponent>( shipEntity );

    TextureInfo textureInfo;
    textureInfo.texture = _texture;
    textureInfo.renderLayer = RenderLayers::Foreground;
    textureInfo.renderSize = { 50.0f, 50.0f };
    textureInfo.offset = { Position(), -PI / 2 };
    //textureInfo.textureSample = { {0.6883f, 0.6048f}, {0.355f, 0.366f} };
    textureInfo.textureSample = { {0.6883f, 0.9400f}, {0.355f, 0.699f} };

    renderComponent->addTextureInfo( textureInfo );
    gSystems->getSystem<IRenderSystem>()->registerEntity( shipEntity );

    //Ship
    componentProvider->addComponent<ShipComponent>( shipEntity );
    gSystems->getSystem<ShipMovementSystem>()->setShipEntity(shipEntity);

    return shipEntity;
}

void ShipSpawner::unspawnShip( Entity _entity )
{
    EntityProvider* entityProvider = gEntities;
    ComponentProvider* componentProvider = gComponents;

    gSystems->getSystem<ICollisionSystem>()->unregisterEntity( _entity );

    componentProvider->removeComponent<ILocationComponent>( _entity );
    componentProvider->removeComponent<IInputComponent>( _entity );
    componentProvider->removeComponent<ICollisionComponent>( _entity );

    gSystems->getSystem<IInputSystem>()->unregisterEntity( _entity );

    entityProvider->disposeEntity( _entity );
}