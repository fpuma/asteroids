#include <precompiledengine.h>

#include "commonlayer.h"

#include <engine/ecs/systems/irendersystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/ecs/components/icameracomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/services/ecsservice.h>
#include <engine/services/systemsservice.h>
#include <engine/services/iengineapplicationservice.h>

#include <asteroids/fakedata/data.h>

namespace ast
{
    CommonLayer::CommonLayer()
        : ILayer(gData->kGameLayers.CommonLayer)
    {}

    void CommonLayer::onInit()
    {
        auto sysProvider = gSystems;

        sysProvider->requestSystem<IRenderSystem>();
        sysProvider->requestSystem<IInputSystem>();

        initCamera();
    }

    void CommonLayer::onUninit()
    {
        uninitCamera();

        auto sysProvider = gSystems;

        sysProvider->releaseSystem<IRenderSystem>();
        sysProvider->releaseSystem<IInputSystem>();
    }

    void CommonLayer::initCamera()
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        m_cameraEntity = entityProvider->requestEntity();

        auto cameraComponent = componentProvider->add<ICameraComponent>( m_cameraEntity );
        auto locationComponent = componentProvider->add<ILocationComponent>( m_cameraEntity );

        cameraComponent->setMetersPerPixel( 1.0f );
        gEngineApplication->setCameraEntity( m_cameraEntity );
        locationComponent->setPosition( Position() );

    }

    void CommonLayer::uninitCamera()
    {
        pina::EntityProvider* entityProvider = gEntities;
        pina::ComponentProvider* componentProvider = gComponents;

        componentProvider->remove<ICameraComponent>( m_cameraEntity );
        componentProvider->remove<ILocationComponent>( m_cameraEntity );

        entityProvider->disposeEntity( m_cameraEntity );
    }
}