#include <precompiledengine.h>
#include "shipspawner.h"

#include <asteroids/components/shipcomponent.h>
#include <asteroids/components/shootcomponent.h>
#include <asteroids/fakedata/data.h>
#include <asteroids/systems/shipmovementsystem.h>
#include <asteroids/systems/shootsystem.h>

#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/components/iinputcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/ecs/systems/irendersystem.h>
#include <utils/geometry/geometryhelpers.h>

#include <engine/services/iengineapplicationservice.h>

namespace ast
{
    Entity ShipSpawner::spawnShip( Position _pos )
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        Entity shipEntity = entityProvider->requestEntity();

        //Location
        componentProvider->addComponent<ILocationComponent>( shipEntity );

        //Input
        auto inputComponent = componentProvider->addComponent<IInputComponent>( shipEntity );
        for (const auto& inputMap : gData->kShipControllerJoystickInput)
        {
            inputComponent->addInputMap( inputMap.inputAction, inputMap.joystickInput );
        }

        for (const auto& inputMap : gData->kShipControllerButtonInput)
        {
            inputComponent->addInputMap( inputMap.inputAction, inputMap.buttonInput );
        }

        for (const auto& inputMap : gData->kShipMouseButtonInput)
        {
            inputComponent->addInputMap( inputMap.inputAction, inputMap.buttonInput );
        }

        inputComponent->addInputMap( gData->kMouseInputMap.first, gData->kMouseInputMap.second );

        for (const auto& inputMap : gData->kShipKeyboardKeyInput)
        {
            inputComponent->addInputMap( inputMap.inputAction, inputMap.keyInput );
        }

        gSystems->getSystem<IInputSystem>()->registerEntity( shipEntity );

        //Collision
        auto collisionComponent = componentProvider->addComponent<ICollisionComponent>( shipEntity );

        leo::FrameInfo frameInfo;
        frameInfo.linearDamping = 0.1f;
        gSystems->getSystem<ICollisionSystem>()->registerEntity( shipEntity, frameInfo, leo::FrameType::Dynamic );

        leo::BodyInfo bodyInfo;
        Circle circle = { Vec2(), gData->kShipInfo.radius };
        bodyInfo.shape.setAsCircle( circle );
        bodyInfo.density = gData->kShipInfo.density;
        bodyInfo.collisionIndex = gData->kCollisionIndexes.Ship;
        bodyInfo.userData = (void*)shipEntity.value();
        collisionComponent->addBody( bodyInfo );

        //Render
        auto renderComponent = componentProvider->addComponent<IRenderComponent>( shipEntity );

        TextureInfo textureInfo;
        textureInfo.texture = gData->kTextureHandles.ShipTexture;
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.renderSize = { gData->kShipInfo.radius*2, gData->kShipInfo.radius*2 };
        textureInfo.offset = { Position(), -PI / 2 };
        //textureInfo.textureSample = { {0.6883f, 0.6048f}, {0.355f, 0.366f} };
        textureInfo.textureSample = { {0.6883f, 0.9400f}, {0.355f, 0.699f} };

        renderComponent->addTextureInfo( textureInfo );
        gSystems->getSystem<IRenderSystem>()->registerEntity( shipEntity );

        //Ship
        componentProvider->addComponent<ShipComponent>( shipEntity );
        auto shootComponent = componentProvider->addComponent<ShootComponent>( shipEntity );
        shootComponent->setBulletSpeed( gData->kBulletInfo.speed );
        shootComponent->setFireRate( gData->kBulletInfo.fireRate );

        gSystems->getSystem<ShipMovementSystem>()->setShipEntity( shipEntity );
        gSystems->getSystem<ShootSystem>()->registerEntity( shipEntity );

        return shipEntity;
    }

    void ShipSpawner::unspawnShip( Entity _entity )
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        gSystems->getSystem<ICollisionSystem>()->unregisterEntity( _entity );
        gSystems->getSystem<IRenderSystem>()->unregisterEntity( _entity );
        gSystems->getSystem<ShootSystem>()->unregisterEntity( _entity );

        componentProvider->removeComponent<ILocationComponent>( _entity );
        componentProvider->removeComponent<IInputComponent>( _entity );
        componentProvider->removeComponent<ICollisionComponent>( _entity );
        componentProvider->removeComponent<IRenderComponent>( _entity );
        componentProvider->removeComponent<ShootComponent>( _entity );
        componentProvider->removeComponent<ShipComponent>( _entity );

        gSystems->getSystem<IInputSystem>()->unregisterEntity( _entity );

        entityProvider->disposeEntity( _entity );
    }
}