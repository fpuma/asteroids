#pragma once

#include <modules/pina/component.h>
#include <utils/geometry/vec2.h>
#include <utils/geometry/geometryhelpers.h>

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

    private:

        float m_thrust = 500.0f;
        float m_turningSpeed = PI;
        float m_desiredAngle = 0.0f;
        Vec2 m_currentForce;

    };
}