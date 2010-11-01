/*#include "Precompile.h"*/
#include "Core/SceneGraph/SceneNodeType.h"
#include "Core/SceneGraph/SceneNode.h"
#include "Foundation/Container/Insert.h" 
#include "Core/Asset/Manifests/SceneManifest.h"

using namespace Helium;
using namespace Helium::SceneGraph;

REFLECT_DEFINE_ABSTRACT( SceneGraph::SceneNodeType );

void SceneNodeType::InitializeType()
{
    Reflect::RegisterClassType< SceneGraph::SceneNodeType >( TXT( "SceneGraph::SceneNodeType" ) );
}

void SceneNodeType::CleanupType()
{
    Reflect::UnregisterClassType< SceneGraph::SceneNodeType >();
}

SceneNodeType::SceneNodeType(SceneGraph::Scene* scene, int32_t instanceType)
: m_Scene( scene )
, m_InstanceType ( instanceType )
, m_ImageIndex( -1 )
{

}

SceneNodeType::~SceneNodeType()
{

}

SceneGraph::Scene* SceneNodeType::GetScene()
{
    return m_Scene;
}

const tstring& SceneNodeType::GetName() const
{
    return m_Name;
}

void SceneNodeType::SetName( const tstring& name )
{
    m_Name = name;
}

int32_t SceneNodeType::GetImageIndex() const
{
    return m_ImageIndex;
}

void SceneNodeType::SetImageIndex( int32_t index )
{
    m_ImageIndex = index;
}

void SceneNodeType::Reset()
{
    m_Instances.clear();
}

void SceneNodeType::AddInstance(SceneNodePtr n)
{
    if ( n->GetNodeType() != this )
    {
        n->SetNodeType( this );

        Helium::Insert<HM_SceneNodeSmartPtr>::Result inserted = m_Instances.insert( HM_SceneNodeSmartPtr::value_type( n->GetID(), n ) );
        HELIUM_ASSERT( inserted.second );

        if ( !n->IsTransient() )
        {
            m_NodeAdded.Raise( n.Ptr() );
        }
    }
}

void SceneNodeType::RemoveInstance(SceneNodePtr n)
{
    m_NodeRemoved.Raise( n.Ptr() );

    m_Instances.erase( n->GetID() );

    n->SetNodeType( NULL );
}

const HM_SceneNodeSmartPtr& SceneNodeType::GetInstances() const
{
    return m_Instances;
}

int32_t SceneNodeType::GetInstanceType() const
{
    return m_InstanceType;
}

void SceneNodeType::PopulateManifest( Asset::SceneManifest* manifest ) const
{
    HM_SceneNodeSmartPtr::const_iterator instItr = m_Instances.begin();
    HM_SceneNodeSmartPtr::const_iterator instEnd = m_Instances.end();
    for ( ; instItr != instEnd; ++instItr )
    {
        instItr->second->PopulateManifest(manifest);
    }
}