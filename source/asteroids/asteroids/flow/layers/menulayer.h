#pragma once

#include <asteroids/flow/layers/base/ilayer.h>
#include <modules/pina/entity.h>

namespace ast
{
    class MenuLayer : public ILayer
    {
    public:

        MenuLayer();
        virtual ~MenuLayer() = default;

        Entity getMenuControllerEntity() const { return m_menuController; }

    private:

        void onInit() override;
        void onUninit() override;

        Entity m_menuController;
        Entity m_pressAToContinue;
    };
}