#pragma once

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

        struct AsteroidsInputActions
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

    };
}

#include <utils/defaultinstance.h>

#define gData puma::DefaultInstance<ast::Data>::getInstance()