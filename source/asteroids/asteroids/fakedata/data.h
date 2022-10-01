#pragma once

#include <modules/leo/leodefinitions.h>
#include <engine/utils/renderdefinitions.h>
#include <engine/input/inputdefinitions.h>
#include <modules/nina/ninadefinitions.h>
#include <array>

using namespace puma;

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

    struct InputActionControllerButtonMap
    {
        InputActionControllerButtonMap( puma::InputAction _ia, puma::ControllerButtonInput _ji )
            : inputAction( _ia )
            , buttonInput( _ji )
        {}

        puma::InputAction inputAction;
        puma::ControllerButtonInput buttonInput;
    };

    using InputActionControllerJoystickPairList = std::array<InputActionJoystickMap, 2>;
    using InputActionControllerButtonPairList = std::array<InputActionControllerButtonMap, 2>;

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
            const puma::InputAction Shoot = puma::InputAction( 2 );
            const puma::InputAction StopShoot = puma::InputAction( 3 );
        } kInputActions;


        const InputActionControllerJoystickPairList kShipControllerJoystickInput =
        {
            { 
                InputActionJoystickMap( kInputActions.MoveShip, {puma::nina::ControllerJoystick::CJ_LSTICK, 0} ),
                InputActionJoystickMap( kInputActions.RotateShip, {puma::nina::ControllerJoystick::CJ_RSTICK, 0} ),
            }
        };

        const InputActionControllerButtonPairList kShipControllerButtonInput =
        {
            {
                InputActionControllerButtonMap( kInputActions.Shoot, {puma::nina::ControllerButton::CB_RB, 0, nina::InputButtonEvent::Pressed} ),
                InputActionControllerButtonMap( kInputActions.StopShoot, {puma::nina::ControllerButton::CB_RB, 0, nina::InputButtonEvent::Released} ),
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
            const float oobLength = 1100.0f;
        }kSpatialCageInfo;

        const struct BulletInfo
        {
            const float speed = 500.0f;
            const float radius = 10.0f;
            const float fireRate = 5.0f;
        }kBulletInfo;

        const struct RockInfo
        {
            const float speed = 50.0f;
            const float radius = 50.0f;
            const float spawnRate = 1.0f;
        }kRockInfo;

        const struct TexturePaths
        {
            const char* ShipSprite = "../assets/asteroids/FighterPlaneV2.png";
            const char* BackgroundTexture = "../assets/asteroids/backgroundSpace_01.1.png";
            const char* RockTexture = "../assets/asteroids/rocks/Asteroids_128x128_001.png";
        }kTexturePaths;
    };
}

#include <utils/defaultinstance.h>

#define gData puma::DefaultInstance<ast::Data>::getInstance()