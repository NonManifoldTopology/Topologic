#include <TopologyFactoryManager.h>
#include <TopologyFactory.h>
#include <ClusterFactory.h>
#include <CellComplexFactory.h>
#include <CellFactory.h>
#include <ShellFactory.h>
#include <FaceFactory.h>
#include <WireFactory.h>
#include <EdgeFactory.h>
#include <VertexFactory.h>

namespace TopologicCore
{
	void TopologyFactoryManager::Add(const std::string& rkGuid, const TopologyFactory::Ptr& rkTopologyFactory)
	{
		if (m_topologyFactoryMap.find(rkGuid) == m_topologyFactoryMap.end())
		{
			m_topologyFactoryMap.insert(std::pair<std::string, TopologyFactory::Ptr>(rkGuid, rkTopologyFactory));
		}
	}

	bool TopologyFactoryManager::Find(const std::string& rkGuid, TopologyFactory::Ptr& rTopologyFactory)
	{
		if (m_topologyFactoryMap.find(rkGuid) != m_topologyFactoryMap.end())
		{
			rTopologyFactory = m_topologyFactoryMap[rkGuid];
			return true;
		}

		return false;
	}

	TopologyFactory::Ptr TopologyFactoryManager::GetDefaultFactory(const TopAbs_ShapeEnum kOcctType)
	{
		switch (kOcctType)
		{
		case TopAbs_COMPOUND: return std::make_shared<ClusterFactory>();
		case TopAbs_COMPSOLID: return std::make_shared<CellComplexFactory>();
		case TopAbs_SOLID: return std::make_shared<CellFactory>();
		case TopAbs_SHELL: return std::make_shared<ShellFactory>();
		case TopAbs_FACE: return std::make_shared<FaceFactory>();
		case TopAbs_WIRE: return std::make_shared<WireFactory>();
		case TopAbs_EDGE: return std::make_shared<EdgeFactory>();
		case TopAbs_VERTEX: return std::make_shared<VertexFactory>();
		default:
			throw std::exception("Topology::ByOcctShape: unknown topology.");
		}
	}
}