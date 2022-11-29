#pragma once

#include <engine/services/base/iservice.h>
#include <asteroids/flow/flowdefinitions.h>
#include <engine/services/base/iservice.h>
#include <utils/containers/uniquerealizationcontainer.h>

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

        const ILayer* getLayer( LayerId _id ) const { return getLayerInternal( _id ); }
        ILayer* getLayer( LayerId _id ) { return getLayerInternal( _id ); }

        template<class T>
        const T* getLayer( LayerId _id ) const { return static_cast<const T*>(getLayer( _id )); }

        template<class T>
        T* getLayer( LayerId _id ) { return static_cast<T*>(getLayer( _id )); }

        void uninit();

    private:

        void removeChildren( ILayer* _layer );

        ILayer* getLayerInternal( LayerId _id ) const
        {
            assert( m_layers.contains( _id ) ); //There is no Layer with that Id

            ILayer* result = nullptr;

            if (m_layers.contains( _id ))
            {
                result = m_layers.at( _id ).get();
            }
            return result;
        }

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