#pragma once

#include <engine/input/inputdefinitions.h>

struct AsteroidsInputActions
{
    static const puma::InputAction MoveShip;
    static const puma::InputAction RotateShip;
};

using InputActionControllerJoystickPairList = std::initializer_list<std::pair<puma::InputAction, puma::ControllerJoystickInput>>;

static const InputActionControllerJoystickPairList kShipControllerJoystickInput =
{
    {AsteroidsInputActions::MoveShip, {puma::nina::ControllerJoystick::CJ_LSTICK, 0}},
    {AsteroidsInputActions::RotateShip, {puma::nina::ControllerJoystick::CJ_RSTICK, 0}},
};