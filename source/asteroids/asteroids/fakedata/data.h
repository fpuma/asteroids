#pragma once

#include <asteroids/flow/flowdefinitions.h>
#include <modules/leo/leodefinitions.h>
#include <engine/utils/renderdefinitions.h>
#include <engine/input/inputdefinitions.h>
#include <modules/nina/ninadefinitions.h>

#include <utils/statemachine/istate.h>
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

    struct InputActionMouseButtonMap
    {
        InputActionMouseButtonMap( puma::InputAction _ia, puma::MouseButtonInput _mi )
            : inputAction( _ia )
            , buttonInput( _mi )
        {}

        puma::InputAction inputAction;
        puma::MouseButtonInput buttonInput;
    };

    struct InputActionKeyboardMap
    {
        InputActionKeyboardMap( puma::InputAction _ia, puma::KeyboardInput _ki )
            : inputAction( _ia )
            , keyInput( _ki )
        {}

        puma::InputAction inputAction;
        puma::KeyboardInput keyInput;
    };

    using InputActionControllerJoystickPairList = std::vector<InputActionJoystickMap>;
    using InputActionControllerButtonPairList = std::vector<InputActionControllerButtonMap>;
    using InputActionMouseButtonPairList = std::vector<InputActionMouseButtonMap>;
    using InputActionKeyboardButtonPairList = std::vector<InputActionKeyboardMap>;

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
            
            const puma::InputAction MouseAimShip = puma::InputAction( 4 );

            const puma::InputAction StartMoveShipUp = puma::InputAction( 5 );
            const puma::InputAction StartMoveShipDown = puma::InputAction( 6 );
            const puma::InputAction StartMoveShipLeft = puma::InputAction( 7 );
            const puma::InputAction StartMoveShipRight = puma::InputAction( 8 );

            const puma::InputAction StopMoveShipUp = puma::InputAction( 9 );
            const puma::InputAction StopMoveShipDown = puma::InputAction( 10 );
            const puma::InputAction StopMoveShipLeft = puma::InputAction( 11 );
            const puma::InputAction StopMoveShipRight = puma::InputAction( 12 );

            const puma::InputAction MenuAButton = puma::InputAction( 13 );

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

        const InputActionMouseButtonPairList kShipMouseButtonInput =
        {
            {
                InputActionMouseButtonMap( kInputActions.Shoot,     {puma::nina::MouseButton::MB_LEFT, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Pressed} ),
                InputActionMouseButtonMap( kInputActions.StopShoot, {puma::nina::MouseButton::MB_LEFT, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Released} ),
            }
        };

        const InputActionKeyboardButtonPairList kShipKeyboardKeyInput =
        {
            {
                InputActionKeyboardMap( kInputActions.StartMoveShipUp,       { nina::KeyboardKey::KB_W, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Pressed } ),
                InputActionKeyboardMap( kInputActions.StartMoveShipDown ,    { nina::KeyboardKey::KB_S, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Pressed } ),
                InputActionKeyboardMap( kInputActions.StartMoveShipLeft ,    { nina::KeyboardKey::KB_A, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Pressed } ),
                InputActionKeyboardMap( kInputActions.StartMoveShipRight ,   { nina::KeyboardKey::KB_D, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Pressed } ),

                InputActionKeyboardMap( kInputActions.StopMoveShipUp,       { nina::KeyboardKey::KB_W, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Released } ),
                InputActionKeyboardMap( kInputActions.StopMoveShipDown ,    { nina::KeyboardKey::KB_S, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Released } ),
                InputActionKeyboardMap( kInputActions.StopMoveShipLeft ,    { nina::KeyboardKey::KB_A, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Released } ),
                InputActionKeyboardMap( kInputActions.StopMoveShipRight ,   { nina::KeyboardKey::KB_D, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Released } ),
            }
        };

        const InputActionKeyboardButtonPairList kMenuKeyboardKeyInput =
        {
            {
                InputActionKeyboardMap( kInputActions.MenuAButton,       { nina::KeyboardKey::KB_A, puma::InputModifier::InputModifier_IGNORE, nina::InputButtonEvent::Pressed } ),
            }
        };

        const std::pair<puma::InputAction, puma::MousePositionInput> kMouseInputMap = { kInputActions.MouseAimShip, {puma::InputModifier_IGNORE} };

        const struct CollisionIndexes
        {
            const puma::leo::CollisionIndex Ship = 0;
            const puma::leo::CollisionIndex SpatialCage = 1;
            const puma::leo::CollisionIndex Asteroid = 2;
            const puma::leo::CollisionIndex ShipSkill = 3;
            const puma::leo::CollisionIndex Playfield = 4;
        }kCollisionIndexes;

        const puma::leo::CollisionCompatibility kCollisionCompatibility =
        {
            {kCollisionIndexes.Ship, kCollisionIndexes.SpatialCage},
            {kCollisionIndexes.ShipSkill, kCollisionIndexes.Playfield},
            {kCollisionIndexes.Asteroid, kCollisionIndexes.Playfield},
            {kCollisionIndexes.Asteroid, kCollisionIndexes.ShipSkill},
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

        const struct ResourcesPaths
        {
            const char* ShipSprite = "../assets/asteroids/FighterPlaneV2.png";
            const char* BackgroundTexture = "../assets/asteroids/backgroundSpace_01.1.png";
            const char* RockTexture = "../assets/asteroids/rocks/Asteroids_128x128_001.png";
            const char* FontPath = "../assets/asteroids/Blitztark_v0-Regular.ttf";
        }kResourcesPaths;

        struct ResourcesHandles
        {
            nina::Texture ShipTexture;
            nina::Texture BackgroundTexture;
            nina::Texture RockTexture;
            nina::Texture PressAnyButtonTexture;

            nina::FontHandle Font;
        }kResourcesHandles;

        const struct GameLayers
        {
            const LayerId CommonLayer = LayerId( 0 );
            const LayerId MenuLayer = LayerId( 1 );
            const LayerId GameplayLayer = LayerId( 2 );
        }kGameLayers;

        const struct GameStates
        {
            const StateId MenuState = StateId( 0 );
            const StateId GameplayState = StateId( 1 );
        }kGameStates;
    };
}

#include <utils/defaultinstance.h>

#define gData puma::DefaultInstance<ast::Data>::getInstance()