#pragma once

#include <modules/pina/component.h>
#include <utils/numerictypes.h>

using namespace puma;

namespace ast
{
    class ImpactComponent : public Component
    {
    public:

        void setDefaultHp( s32 _value ) { m_defaultHp = _value; }
        s32 getDefaultHp() const { return m_defaultHp; }

        void setCurrentHp( s32 _value ) { m_currentHp = _value; }
        s32  getCurrentHp() const { return m_currentHp; }

        void setDamage( s32 _value ) { m_damage = _value; }
        s32 getDamage() const { return m_damage; }

    private:

        s32 m_defaultHp = 1;
        s32 m_currentHp = 1;

        s32 m_damage = 1;

    };
}