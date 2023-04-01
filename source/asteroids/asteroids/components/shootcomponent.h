#pragma once

#include <pina/component.h>
#include <engine/utils/cooldown.h>

using namespace puma;

namespace ast
{
    class ShootComponent : public pina::Component
    {
    public:

        void setBulletSpeed( float _speed ) { m_bulletSpeed = _speed; }
        float getBulletSpeed() const { return m_bulletSpeed; }

        void setFireRate( float _rate ) { m_fireRate = _rate; m_shotCooldown.setCooldownTime( 1.0f / m_fireRate ); }
        float getFireRate() const { return m_fireRate; }

        void engage() { m_engage = true; }
        void disengage() { m_engage = false; }

        bool isEngaged() const { return m_engage; }

        bool tryShoot() 
        { 
            return m_shotCooldown.cooldownReady();
        }

    private:

        enum class ShotState
        {
            CanShoot = 0,
            OnCooldown = 1,
        } m_state = ShotState::CanShoot;

        float m_bulletSpeed = 50.0f;
        float m_fireRate = 2.0f;
        Cooldown m_shotCooldown;
        bool m_engage = false;
    };
}