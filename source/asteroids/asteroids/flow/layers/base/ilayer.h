#pragma once

#include <utils/genericid.h>
#include <utils/numerictypes.h>
#include <asteroids/flow/layers/base/layermanager.h>

using namespace puma;

namespace ast
{

    class ILayer
    {
    public:

        ILayer( LayerId _id, LayerId _parent = {} ) : m_id( _id ), m_parent( _parent ) {}
        virtual ~ILayer() = default;

        LayerId getId() const { return m_id; }
        LayerId getParentId() const { return m_parent; }

        const std::set<LayerId>& getChildren() const { return m_children; }

        bool operator < ( const LayerId& _other ) { m_id < _other; }

    private:

        friend class LayerManager;

        void addChild( LayerId _id ) { m_children.insert( _id ); }
        void removeChild( LayerId _id ) { m_children.erase( _id ); }

        virtual void onInit() = 0;
        virtual void onUninit() = 0;

        LayerId m_id;
        LayerId m_parent;
        std::set<LayerId> m_children;
    };
}