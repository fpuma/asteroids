#include <precompiledengine.h>

#include "commonlayer.h"

#include <engine/ecs/systems/irendersystem.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/ecs/components/icameracomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/services/ecsservice.h>
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

        sysProvider->addSystem<IRenderSystem>();
        sysProvider->addSystem<IInputSystem>();

        initCamera();
    }

    void CommonLayer::onUninit()
    {
        uninitCamera();

        auto sysProvider = gSystems;

        sysProvider->removeSystem<IRenderSystem>();
        sysProvider->removeSystem<IInputSystem>();
    }

    void CommonLayer::initCamera()
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        m_cameraEntity = entityProvider->requestEntity();

        auto cameraComponent = componentProvider->addComponent<ICameraComponent>( m_cameraEntity );
        auto locationComponent = componentProvider->addComponent<ILocationComponent>( m_cameraEntity );

        cameraComponent->setMetersPerPixel( 1.0f );
        gEngineApplication->setCameraEntity( m_cameraEntity );
        locationComponent->setPosition( Position() );

    }

    void CommonLayer::uninitCamera()
    {
        EntityProvider* entityProvider = gEntities;
        ComponentProvider* componentProvider = gComponents;

        componentProvider->removeComponent<ICameraComponent>( m_cameraEntity );
        componentProvider->removeComponent<ILocationComponent>( m_cameraEntity );

        entityProvider->disposeEntity( m_cameraEntity );
    }
}