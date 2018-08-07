#include <TopologyFactoryManager.h>
#include <CellComplexFactory.h>
#include <FaceFactory.h>
#include <VertexFactory.h>
#include <ApertureFactory.h>

namespace Topologic
{
	void TopologyFactoryManager::Add(const TopologicCore::Topology::Ptr& content, TopologyFactory^ value)
	{
		Add(gcnew String(content->GetClassGUID().c_str()), value);
	}

	void TopologyFactoryManager::Add(String^ key, TopologyFactory^ value)
	{
		try {
			m_TopologyFactoryDict->Add(key, value);
		}
		catch (Exception^)
		{

		}
	}

	TopologyFactory^ TopologyFactoryManager::Find(String^ key)
	{
		if (m_TopologyFactoryDict->ContainsKey(key))
		{
			return m_TopologyFactoryDict[key];
		}
		else
		{
			throw gcnew Exception("Not found");
		}
	}

	TopologyFactory^ TopologyFactoryManager::GetDefaultFactory(const TopologicCore::Topology::Ptr & content)
	{
		switch (content->GetType())
		{
		case TopologicCore::TOPOLOGY_CLUSTER: throw gcnew NotImplementedException();// return gcnew ClusterFactory();
		case TopologicCore::TOPOLOGY_CELLCOMPLEX: return gcnew CellComplexFactory();
		case TopologicCore::TOPOLOGY_CELL: throw gcnew NotImplementedException(); //return std::make_shared<CellFactory>();
		case TopologicCore::TOPOLOGY_SHELL: throw gcnew NotImplementedException(); //return std::make_shared<ShellFactory>();
		case TopologicCore::TOPOLOGY_FACE: return gcnew FaceFactory();
		case TopologicCore::TOPOLOGY_WIRE: throw gcnew NotImplementedException(); //return std::make_shared<WireFactory>();
		case TopologicCore::TOPOLOGY_EDGE: throw gcnew NotImplementedException(); //return std::make_shared<EdgeFactory>();
		case TopologicCore::TOPOLOGY_VERTEX: return gcnew VertexFactory();
		case TopologicCore::TOPOLOGY_APERTURE: throw gcnew NotImplementedException(); //return std::make_shared<DualGraphFactory>();
		case TopologicCore::TOPOLOGY_DUALGRAPH: return gcnew ApertureFactory();
		default:
			throw gcnew Exception("Topology::ByOcctShape: unknown topology.");
		}
	}
}