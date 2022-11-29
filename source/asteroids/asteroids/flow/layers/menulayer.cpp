#include <precompiledengine.h>

#include "menulayer.h"

#include <asteroids/fakedata/data.h>

#include <engine/ecs/components/iinputcomponent.h>
#include <engine/ecs/components/irendercomponent.h>
#include <engine/ecs/components/ilocationcomponent.h>
#include <engine/ecs/systems/iinputsystem.h>
#include <engine/ecs/systems/irendersystem.h>
#include <engine/services/ecsservice.h>

namespace ast
{
    
    MenuLayer::MenuLayer()
        : ILayer(gData->kGameLayers.MenuLayer, gData->kGameLayers.CommonLayer)
    {

    }

    void MenuLayer::onInit()
    {
        m_menuController = gEntities->requestEntity();

        auto inputComponent = gComponents->addComponent<IInputComponent>( m_menuController );

        for (const auto& inputMap : gData->kMenuKeyboardKeyInput)
        {
            inputComponent->addInputMap( inputMap.inputAction, inputMap.keyInput );
        }

        gSystems->getSystem<IInputSystem>()->registerEntity( m_menuController );


        m_pressAToContinue = gEntities->requestEntity();

        auto renderComponent = gComponents->addComponent<IRenderComponent>( m_pressAToContinue );

        TextureInfo textureInfo;
        textureInfo.renderLayer = gData->kRenderLayers.Foreground;
        textureInfo.texture = gData->kResourcesHandles.PressAnyButtonTexture;
        auto originalSize = gData->kResourcesHandles.PressAnyButtonTexture.getOriginalSize();
        float ratio = static_cast<float>(originalSize.width) / static_cast<float>(originalSize.height);
        textureInfo.renderSize = { 50.0f * ratio, 50.0f };
        renderComponent->addTextureInfo( textureInfo );
        
        auto locationComponent = gComponents->addComponent<ILocationComponent>( m_pressAToContinue );
        locationComponent->setPosition( { 0.0f, 0.0f } );

        gSystems->getSystem<IRenderSystem>()->registerEntity( m_pressAToContinue );
    }

    void MenuLayer::onUninit()
    {
        gSystems->getSystem<IRenderSystem>()->unregisterEntity( m_pressAToContinue );
        gComponents->removeComponent<ILocationComponent>( m_pressAToContinue );
        gComponents->removeComponent<IRenderComponent>( m_pressAToContinue );
        gEntities->disposeEntity( m_pressAToContinue );

        gSystems->getSystem<IInputSystem>()->unregisterEntity( m_menuController );
        gComponents->removeComponent<IInputComponent>( m_menuController );
        gEntities->disposeEntity( m_menuController );

    }
}