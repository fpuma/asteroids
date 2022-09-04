#pragma once

#include <modules/pina/entity.h>
#include <engine/igame.h>
#include <asteroids/fakedata/data.h>

using namespace puma;
namespace ast
{
    class Asteroids : public IGame
    {
    public:

        void init() override;
        void uninit() override;
        void update( float _deltaTime ) override;

    private:

        void initCamera();
        void uninitCamera();
        void initPhysics();

        Entity m_cameraEntity;
        Entity m_shipEntity;
        Entity m_backgroundEntity;

        std::unique_ptr<Data> m_data;
    };
}