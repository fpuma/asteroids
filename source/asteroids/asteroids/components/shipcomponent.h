#pragma once

#include <modules/pina/component.h>
#include <utils/geometry/vec2.h>
#include <utils/geometry/geometryhelpers.h>

#include <bitset>

using namespace puma;

namespace ast
{
    class ShipComponent : public Component
    {
    public:

        void setDirection( Vec2 _direction ) { m_currentForce = _direction * m_thrust; }

        Vec2 getCurrentForce() const { return m_currentForce; }

        void setThrust( float _thrust ) { m_thrust = _thrust; }
        float getThrust() const { return m_thrust; }

        void setTurningSpeed( float _ts ) { m_turningSpeed = _ts; }
        float getTurningSpeed() const { return m_turningSpeed; }

        void setDesiredeAngle( float _angle ) { m_desiredAngle = _angle; }
        float getDesiredAngle() const { return m_desiredAngle; }

        void moveUp( bool _value ) { if (_value) m_moveMask.set( Up ); else m_moveMask.reset( Up ); }
        void moveDown( bool _value ) { if (_value) m_moveMask.set( Down ); else m_moveMask.reset( Down ); }
        void moveLeft( bool _value ) { if (_value) m_moveMask.set( Left ); else m_moveMask.reset( Left ); }
        void moveRight( bool _value ) { if (_value) m_moveMask.set( Right ); else m_moveMask.reset( Right ); }

        void updateDirection()
        {
            Vec2 dir;
            if (m_moveMask[Up]) dir = dir + Vec2( 0.0f, 1.0f );
            if (m_moveMask[Down]) dir = dir + Vec2( 0.0f, -1.0f );
            if (m_moveMask[Left]) dir = dir + Vec2( -1.0f, 0.0f );
            if (m_moveMask[Right]) dir = dir + Vec2( 1.0f, 0.0f );

            setDirection( dir );
        }

    private:

        float m_thrust = 500000.0f;
        float m_turningSpeed = 2*PI;
        float m_desiredAngle = 0.0f;
        Vec2 m_currentForce;
        
        enum
        {
            Up = 0,
            Down,
            Left,
            Right
        };

        std::bitset<4> m_moveMask;
    };
}