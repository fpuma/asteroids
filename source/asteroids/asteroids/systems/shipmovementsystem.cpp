#include <precompiledengine.h>

#include "shipmovementsystem.h"

#include <asteroids/fakedata/data.h>
#include <asteroids/components/shipcomponent.h>
#include <asteroids/components/impactcomponent.h>

#include <engine/ecs/components/iinputcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
#include <utils/geometry/geometryhelpers.h>
#include <engine/services/iloggerservice.h>
#include <utils/formatstring.h>
#include <engine/renderer/irenderqueue.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/renderer/renderhelpers.h>

namespace ast
{
    void ShipMovementSystem::onInit()
    {
        gSystems->subscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::PrePhysics );
        gSystems->subscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::QueueRenderables );
    }

    void ShipMovementSystem::onUninit()
    {
        gSystems->unsubscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::PrePhysics );
        gSystems->unsubscribeSystemUpdate<ShipMovementSystem>( SystemUpdateId::QueueRenderables );
    }

    namespace
    {
        void handleKbMovement( IInputComponent* _ic, ShipComponent* _sc )
        {
            bool moveByKb = false;

            if (_ic->isActionActive( gData->kInputActions.StartMoveShipUp )) 
            { 
                moveByKb = true; 
                _sc->moveUp( true );
            }
            if (_ic->isActionActive( gData->kInputActions.StartMoveShipDown ))  
            { 
                moveByKb = true; 
                _sc->moveDown( true );
            }
            if (_ic->isActionActive( gData->kInputActions.StartMoveShipLeft ))  
            { 
                moveByKb = true; 
                _sc->moveLeft( true );
            }
            if (_ic->isActionActive( gData->kInputActions.StartMoveShipRight )) 
            { 
                moveByKb = true; 
                _sc->moveRight( true );
            }
            //-------------------------------------------------------------------------------
            if (_ic->isActionActive( gData->kInputActions.StopMoveShipUp )) 
            { 
                moveByKb = true; 
                _sc->moveUp( false );
            }
            if (_ic->isActionActive( gData->kInputActions.StopMoveShipDown )) 
            { 
                moveByKb = true; 
                _sc->moveDown( false );
            }
            if (_ic->isActionActive( gData->kInputActions.StopMoveShipLeft )) 
            { 
                moveByKb = true; 
                _sc->moveLeft( false );
            }
            if (_ic->isActionActive( gData->kInputActions.StopMoveShipRight )) 
            { 
                moveByKb = true; 
                _sc->moveRight( false );
            }

            if (moveByKb)
            {
                _sc->updateDirection();
            }
        }
    }

    void ShipMovementSystem::prePhysicsUpdate( pina::EntityProvider& _entityProvider, pina::ComponentProvider& _componentProvider )
    {
        if (!gEntities->isEntityEnabled( m_shipEntity )) return;

        IInputComponent* inputComponent = _componentProvider.get<IInputComponent>( m_shipEntity );
        ShipComponent* shipComponent = _componentProvider.get<ShipComponent>( m_shipEntity );

        ICollisionComponent* collisionComponent = _componentProvider.get<ICollisionComponent>( m_shipEntity );
        leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();

        //Process controller input
        if (inputComponent->isActionActive( gData->kInputActions.MoveShip ))
        {
            InputActionExtraInfo extraInfo = inputComponent->getInputActionExtraInfo( gData->kInputActions.MoveShip );

            Vec2 direction = { extraInfo.x, extraInfo.y };

            shipComponent->setDirection( direction );
        }

        if (inputComponent->isActionActive( gData->kInputActions.RotateShip ))
        {
            InputActionExtraInfo extraInfo = inputComponent->getInputActionExtraInfo( gData->kInputActions.RotateShip );

            Vec2 direction = { extraInfo.x, extraInfo.y };
            if (direction.length() > 0.5f)
            {
                //New angle
                direction = direction.normalize();
                float newAngle = std::acos( direction.x );
                newAngle = extraInfo.y < 0.0f ? 2.0f * PI - newAngle : newAngle;

                shipComponent->setDesiredeAngle( newAngle );
            }
        }
        //-----------------------------------

        //Process mouse and keyboard input

        handleKbMovement( inputComponent, shipComponent );

        if (inputComponent->isActionActive( gData->kInputActions.MouseAimShip ))
        {
            InputActionExtraInfo extraInfo = inputComponent->getInputActionExtraInfo( gData->kInputActions.MouseAimShip );

            Position worldMousePos = erh::screenPointToWorld( { static_cast<s32>(extraInfo.x), static_cast<s32>(extraInfo.y) } );

            Vec2 direction = worldMousePos - frame->getPosition();

            //New angle
            direction = direction.normalize();
            float newAngle = std::acos( direction.x );
            newAngle = worldMousePos.y < frame->getPosition().y ? 2.0f * PI - newAngle : newAngle;

            shipComponent->setDesiredeAngle( newAngle );
        }

        //-----------------------------------

        Vec2 currentForce = shipComponent->getCurrentForce();
        if (currentForce.length() > 0.0f)
        {
            frame->applyForceToCenter( currentForce );
        }

        //Update angle
        float desiredAngle = shipComponent->getDesiredAngle();

        float currentAngle = processCurrentAngle( frame->getAngle() );
        float deltaAngle = desiredAngle - currentAngle;
        float absDeltaAngle = std::fabsf( deltaAngle );

        if (absDeltaAngle < 0.07f)
        {
            frame->setAngle( desiredAngle );
            frame->setAngularVelocity( 0.0f );
        }
        else
        {
            float angularSign = deltaAngle < 0 ? -1.0f : 1.0f;
            angularSign = absDeltaAngle > PI ? angularSign * -1.0f : angularSign;

            float angularVelocity = shipComponent->getTurningSpeed() * angularSign;
            frame->setAngularVelocity( angularVelocity );
        }
    }

    float ShipMovementSystem::processCurrentAngle( float _rawAngle )
    {
        float dummy = 0.0f;
        float fraction = std::modf( _rawAngle / (2.0f * PI), &dummy );
        float processedAngle = fraction * 2.0f * PI;
        return processedAngle < 0.0f ? (2.0f * PI) + processedAngle : processedAngle;
    }

    void ShipMovementSystem::queueRenderables( IRenderQueue& _renderQueue )
    {
        if (!gEntities->isEntityEnabled( m_shipEntity )) return;
        //ICollisionComponent* collisionComponent = gComponents->get<ICollisionComponent>( m_shipEntity );
        ILocationComponent* locationComponent = gComponents->get<ILocationComponent>( m_shipEntity );
        //leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();
        //ShipComponent* shipComponent = gComponents->get<ShipComponent>( m_shipEntity );

        //_renderQueue.addDebugRenderableText( formatString( "AVel: %.4f", frame->getAngularVelocity() ),Color::White(), locationComponent->getPosition() );
        //_renderQueue.addDebugRenderableText( formatString( "RAng: %.4f", frame->getAngle() ), Color::White(), locationComponent->getPosition() + Position( 0.0f, 12.5f ) );
        //_renderQueue.addDebugRenderableText( formatString( "PAng: %.4f", processCurrentAngle( frame->getAngle() ) ), Color::White(), locationComponent->getPosition() + Position( 0.0f, 25.0f ) );
        //_renderQueue.addDebugRenderableText( formatString( "DAng: %.4f", shipComponent->getDesiredAngle() ), Color::White(), locationComponent->getPosition() + Position(0.0f, 37.5f ));

        //_renderQueue.addDebugRenderableText( formatString( "Vel: %.4f", frame->getLinearVelocity().length() ), Color::White(), locationComponent->getPosition() );

        ImpactComponent* impactComponent = gComponents->get<ImpactComponent>( m_shipEntity );
        _renderQueue.addDebugRenderableText( formatString( "HP: %d", impactComponent->getCurrentHp() ), Color::White(), locationComponent->getPosition() );
    }
}
