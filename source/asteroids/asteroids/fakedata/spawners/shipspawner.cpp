#include <precompiledengine.h>
#include "shipspawner.h"

#include <asteroids/components/shipcomponent.h>
#include <asteroids/components/shootcomponent.h>
#include <asteroids/components/impactcomponent.h>
#include <asteroids/fakedata/data.h>
#include <asteroids/systems/shipmovementsystem.h>
#include <asteroids/systems/shootsystem.h>

#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/components/iinputcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
#include <engine/ecs/systems/icollisionsystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/ecs/systems/irendersystem.h>
#include <utils/geometry/geometryhelpers.h>

#include <engine/services/iengineapplicationservice.h>

namespace ast
{
    pina::Entity ShipSpawner::spawnShip( Position _pos )
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        pina::Entity shipEntity = entityProvider->requestEntity();

        //Location
        componentProvider->add<ILocationComponent>( shipEntity );

        //Input
        auto inputComponent = componentProvider->add<IInputComponent>( shipEntity );
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

        //Collision
        auto collisionComponent = componentProvider->add<ICollisionComponent>( shipEntity );

        leo::FrameInfo frameInfo;
        frameInfo.linearDamping = 0.1f;
        collisionComponent->init( leo::FrameType::Dynamic, frameInfo );

        leo::BodyInfo bodyInfo;
        Circle circle = { Vec2(), gData->kShipInfo.radius };
        bodyInfo.shape.setAsCircle( circle );
        bodyInfo.density = gData->kShipInfo.density;
        bodyInfo.collisionIndex = gData->kCollisionIndexes.Ship;
        bodyInfo.userData = (void*)shipEntity.value();
        collisionComponent->addBody( bodyInfo );

        //Render
        auto renderComponent = componentProvider->add<IRenderComponent>( shipEntity );

        TextureInfo textureInfo;
        textureInfo.texture = gData->kResourcesHandles.ShipTexture;
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.renderSize = { gData->kShipInfo.radius*2, gData->kShipInfo.radius*2 };
        textureInfo.offset = { Position(), -PI / 2 };
        //textureInfo.textureSample = { {0.6883f, 0.6048f}, {0.355f, 0.366f} };
        textureInfo.textureSample = { {0.6883f, 0.9400f}, {0.355f, 0.699f} };

        renderComponent->addTextureInfo( textureInfo );

        //Ship
        componentProvider->add<ShipComponent>( shipEntity );
        auto shootComponent = componentProvider->add<ShootComponent>( shipEntity );
        shootComponent->setBulletSpeed( gData->kBulletInfo.speed );
        shootComponent->setFireRate( gData->kBulletInfo.fireRate );

        auto impactComponent = componentProvider->add<ImpactComponent>( shipEntity );
        impactComponent->setCurrentHp( 10 );
        impactComponent->setDefaultHp( 10 );
        impactComponent->setDamage( 99 );

        return shipEntity;
    }

    void ShipSpawner::unspawnShip( pina::Entity _entity )
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        componentProvider->remove<ILocationComponent>( _entity );
        componentProvider->remove<IInputComponent>( _entity );
        componentProvider->remove<ICollisionComponent>( _entity );
        componentProvider->remove<IRenderComponent>( _entity );
        componentProvider->remove<ShootComponent>( _entity );
        componentProvider->remove<ShipComponent>( _entity );
        componentProvider->remove<ImpactComponent>( _entity );

        entityProvider->disposeEntity( _entity );
    }
}