#pragma once

#include <engine/services/base/iservice.h>
#include <asteroids/flow/flowdefinitions.h>
#include <engine/services/base/iservice.h>

using namespace puma;

namespace ast
{
    class ILayer;

    class LayerManager : public IService
    {
    public:

        LayerManager();
        ~LayerManager();

        void addLayer( std::unique_ptr<ILayer>&& _layerPtr );
        void removeLayer( LayerId _id );

        void uninit();

    private:

        void removeChildren( ILayer* _layer );

        struct LayerIdHash
        {
            size_t operator()(const LayerId& _id) const { return _id.value(); }
        };

        std::unordered_map<LayerId, std::unique_ptr<ILayer>, LayerIdHash> m_layers;
        std::set<LayerId> m_rootLayers;

    };

    class LayerService : public IService
    {
    public:

        LayerService() { m_layerManager = std::make_unique<LayerManager>(); }

        LayerManager* get() { return m_layerManager.get(); }

    private:

        std::unique_ptr<LayerManager> m_layerManager;
    };

}

#include <engine/services/base/iservicecontainer.h>

#define gLayerManager puma::DefaultServices::getInstance()->get<ast::LayerService>()->get()