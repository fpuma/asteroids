#pragma once

#include <modules/leo/leodefinitions.h>
#include <engine/utils/renderdefinitions.h>
#include <engine/input/inputdefinitions.h>
#include <array>

namespace ast
{

    struct InputActionJoystickMap
    {
        InputActionJoystickMap( puma::InputAction _ia, puma::ControllerJoystickInput _ji )
            : inputAction(_ia)
            , joystickInput(_ji)
        {}

        puma::InputAction inputAction;
        puma::ControllerJoystickInput joystickInput;
    };

    using InputActionControllerJoystickPairList = std::array<InputActionJoystickMap, 2>;

    class Data
    {
    public:

        const struct RenderLayers
        {
            const puma::RenderLayer Background = puma::RenderLayer( 0 );;
            const puma::RenderLayer Foreground = puma::RenderLayer( 1 );;
        } kRenderLayers;

        const struct AsteroidsInputActions
        {
            const puma::InputAction MoveShip = puma::InputAction( 0 );
            const puma::InputAction RotateShip = puma::InputAction( 1 );
        } kInputActions;


        const InputActionControllerJoystickPairList kShipControllerJoystickInput =
        {
            { 
                InputActionJoystickMap( kInputActions.MoveShip, {puma::nina::ControllerJoystick::CJ_LSTICK, 0} ),
                InputActionJoystickMap( kInputActions.RotateShip, {puma::nina::ControllerJoystick::CJ_RSTICK, 0} )
            }
        };

        const struct CollisionIndexes
        {
            const puma::leo::CollisionIndex Ship = 0;
            const puma::leo::CollisionIndex SpatialCage = 1;
            const puma::leo::CollisionIndex Asteroid = 2;
            const puma::leo::CollisionIndex ShipSkill = 3;
        }kCollisionIndexes;

        const puma::leo::CollisionCompatibility kCollisionCompatibility =
        {
            {kCollisionIndexes.Ship, kCollisionIndexes.SpatialCage}
        };

        const struct ShipInfo
        {
            const float thrust = 500000.0f;
            const float linearDamping = 0.1f;
            const float radius = 25.0f;
            const float density = 0.5f;
        }kShipInfo;

        const struct SpatialCageInfo
        {
            const float width = 1920.0f;
            const float height = 1080.0f;
            const float threshold = 50.0f;
        }kSpatialCageInfo;
    };
}

#include <utils/defaultinstance.h>

#define gData puma::DefaultInstance<ast::Data>::getInstance()