#pragma once

#include <asteroids/flow/layers/base/ilayer.h>
#include <pina/entity.h>

namespace ast
{
    class MenuLayer : public ILayer
    {
    public:

        MenuLayer();
        virtual ~MenuLayer() = default;

        pina::Entity getMenuControllerEntity() const { return m_menuController; }

    private:

        void onInit() override;
        void onUninit() override;

        pina::Entity m_menuController;
        pina::Entity m_pressAToContinue;
    };
}