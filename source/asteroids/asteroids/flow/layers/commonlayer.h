#pragma once

#include <asteroids/flow/layers/base/ilayer.h>
#include <pina/entity.h>

namespace ast
{
    class CommonLayer : public ILayer
    {
    public:

        CommonLayer();
        virtual ~CommonLayer() = default;

    private:

        void onInit() override;
        void onUninit() override;

        void initCamera();
        void uninitCamera();

        pina::Entity m_cameraEntity;

    };
}