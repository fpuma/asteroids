#pragma once

#include <asteroids/flow/layers/base/ilayer.h>
#include <pina/entity.h>

namespace ast
{
    class GameplayLayer : public ILayer
    {
    public:

        GameplayLayer();
        virtual ~GameplayLayer() = default;

    private:

        void onInit() override;
        void onUninit() override;

        void initPhysics();

        pina::Entity m_shipEntity;

    };
}