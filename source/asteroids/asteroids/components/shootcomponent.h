#pragma once

#include <modules/pina/component.h>
#include <time/timers/countdowntimer.h>

using namespace puma;

namespace ast
{
    class ShootComponent : public puma::Component
    {
    public:

        void setBulletSpeed( float _speed ) { m_bulletSpeed = _speed; }
        float getBulletSpeed() const { return m_bulletSpeed; }

        void setFireRate( float _rate ) { m_fireRate = _rate; m_shotCooldown.setTimeLimit( 1.0f / m_fireRate ); }
        float getFireRate() const { return m_fireRate; }

        void engage() { m_engage = true; }
        void disengage() { m_engage = false; }

        bool isEngaged() const { return m_engage; }

        bool tryShoot() 
        { 
            bool result = false;
            switch (m_state)
            {
            case ShotState::CanShoot:
            {
                m_state = ShotState::OnCooldown;
                m_shotCooldown.play();
                result = true;
                break;
            }
            case ShotState::OnCooldown:
            {
                if (m_shotCooldown.isFinished())
                {
                    m_shotCooldown.stop();
                    m_shotCooldown.play();
                    result = true;
                }
                break;
            }
            default:
                assert( false ); // Something very wrong happened
            }

            return result;
        }

    private:

        enum class ShotState
        {
            CanShoot = 0,
            OnCooldown = 1,
        } m_state = ShotState::CanShoot;

        float m_bulletSpeed = 50.0f;
        float m_fireRate = 2.0f;
        CountdownTimer m_shotCooldown;
        bool m_engage = false;
    };
}