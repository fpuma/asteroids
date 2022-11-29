#include <precompiledengine.h>
#include "layermanager.h"

#include <asteroids/flow/layers/base/ilayer.h>

using namespace puma;

namespace ast
{

    LayerManager::LayerManager()
    {

    }

    LayerManager::~LayerManager()
    {

    }

    void LayerManager::addLayer( std::unique_ptr<ILayer>&& _layerPtr )
    {
        LayerId layerId = _layerPtr->getId();
        LayerId parentId = _layerPtr->getParentId();
        
        assert( layerId.isValid() ); //Trying to add a layer with an invalid Id

        if (parentId.isValid())
        {
            assert( m_layers.contains( parentId ) ); //Parent layer has not been added yet
            if (m_layers.contains( parentId ))
            {
                m_layers.at( parentId )->addChild( layerId );
            }
        }
        else
        {
            m_rootLayers.insert( layerId );
        }

        _layerPtr->onInit();

        m_layers.insert( { layerId, std::move( _layerPtr ) } );
    }

    void LayerManager::removeLayer( LayerId _id )
    {
        assert( m_layers.contains( _id ) ); //Trying to remove a layer that has not been added

        if (!m_layers.contains( _id )) return; //Early return if there is no layer with the given id

        ILayer* layer = m_layers.at( _id ).get();

        removeChildren( layer );
        
        assert( m_layers.contains( layer->getParentId() ) || m_rootLayers.contains(_id) ); // Removing a layer with a non existent parent?

        if (m_layers.contains( layer->getParentId() ))
        {
            m_layers.at( layer->getParentId() )->removeChild( _id );
        }

        layer->onUninit();

        m_layers.erase( _id );
        
        m_rootLayers.erase( _id );
    }

    void LayerManager::uninit()
    {
        for ( const LayerId& layerId : m_rootLayers)
        {
            removeLayer( layerId );
        }
    }

    void LayerManager::removeChildren( ILayer* _layer )
    {
        for (const LayerId& childId : _layer->getChildren())
        {
            assert( m_layers.contains( childId ) ); //This layer has a child layer id of a layer that has already been removed
            if (!m_layers.contains( childId )) continue;

            ILayer* childLayer = m_layers.at( childId ).get();

            removeChildren( childLayer );

            childLayer->onUninit();

            m_layers.erase( childId );
        }
    }

}