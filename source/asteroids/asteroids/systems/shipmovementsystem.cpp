#include <precompiledengine.h>

#include "shipmovementsystem.h"

#include <asteroids/fakedata/input/inputconfig.h>
#include <asteroids/components/shipcomponent.h>

#include <engine/ecs/components/iinputcomponent.h>
#include <engine/ecs/components/icollisioncomponent.h>
#include <engine/services/ecsservice.h>
#include <utils/geometry/geometryhelpers.h>
#include <engine/services/iloggerservice.h>
#include <utils/formatstring.h>
#include <engine/renderer/irenderqueue.h>
#include <engine/ecs/components/ilocationcomponent.h>

ShipMovementSystem::ShipMovementSystem()
{
}

void ShipMovementSystem::prePhysicsUpdate( EntityProvider& _entityProvider, ComponentProvider& _componentProvider )
{
    IInputComponent* inputComponent = _componentProvider.getComponent<IInputComponent>( m_shipEntity );
    ShipComponent* shipComponent = _componentProvider.getComponent<ShipComponent>( m_shipEntity );
    
    ICollisionComponent* collisionComponent = _componentProvider.getComponent<ICollisionComponent>( m_shipEntity );
    leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();

    if ( inputComponent->isActionActive( AsteroidsInputActions::MoveShip ) )
    {
        InputActionExtraInfo extraInfo = inputComponent->getInputActionExtraInfo( AsteroidsInputActions::MoveShip );

        Vec2 direction = { extraInfo.x, extraInfo.y };

        shipComponent->setDirection( direction );
    }

    Vec2 currentForce = shipComponent->getCurrentForce();
    if ( currentForce.length() > 0.0f )
    {
        frame->applyForceToCenter( currentForce );
    }

    if (inputComponent->isActionActive( AsteroidsInputActions::RotateShip ))
    {
        InputActionExtraInfo extraInfo = inputComponent->getInputActionExtraInfo( AsteroidsInputActions::RotateShip );

        Vec2 direction = { extraInfo.x, extraInfo.y };
        if (direction.length() > 0.5f)
        {
            //New angle
            direction = direction.normalize();
            float newAngle = std::acos( direction.x );
            newAngle = extraInfo.y < 0.0f ? 2.0f*PI - newAngle : newAngle;
            
            shipComponent->setDesiredeAngle( newAngle );
        }       
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
    ICollisionComponent* collisionComponent = gComponents->getComponent<ICollisionComponent>( m_shipEntity );
    ILocationComponent* locationComponent = gComponents->getComponent<ILocationComponent>( m_shipEntity );
    leo::IDynamicFrame* frame = collisionComponent->getDynamicFrame();
    //ShipComponent* shipComponent = gComponents->getComponent<ShipComponent>( m_shipEntity );

    //_renderQueue.addDebugRenderableText( formatString( "AVel: %.4f", frame->getAngularVelocity() ),Color::White(), locationComponent->getPosition() );
    //_renderQueue.addDebugRenderableText( formatString( "RAng: %.4f", frame->getAngle() ), Color::White(), locationComponent->getPosition() + Position( 0.0f, 12.5f ) );
    //_renderQueue.addDebugRenderableText( formatString( "PAng: %.4f", processCurrentAngle( frame->getAngle() ) ), Color::White(), locationComponent->getPosition() + Position( 0.0f, 25.0f ) );
    //_renderQueue.addDebugRenderableText( formatString( "DAng: %.4f", shipComponent->getDesiredAngle() ), Color::White(), locationComponent->getPosition() + Position(0.0f, 37.5f ));

    _renderQueue.addDebugRenderableText( formatString( "Vel: %.4f", frame->getLinearVelocity().length() ), Color::White(), locationComponent->getPosition() );
}