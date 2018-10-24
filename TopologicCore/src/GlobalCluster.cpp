#include "GlobalCluster.h"
#include "Topology.h"

namespace TopologicCore
{
	void GlobalCluster::AddTopology(const std::shared_ptr<Topology>& rkTopology)
	{
		AddTopology(rkTopology->GetOcctShape());
	}

	void GlobalCluster::AddTopology(const TopoDS_Shape & rkOcctShape)
	{
		m_occtBuilder.Add(m_occtCompound, rkOcctShape);
	}

	void GlobalCluster::RemoveTopology(const std::shared_ptr<Topology>& rkTopology)
	{
		RemoveTopology(rkTopology->GetOcctShape());
	}

	void GlobalCluster::RemoveTopology(const TopoDS_Shape & rkOcctShape)
	{
		m_occtBuilder.Remove(m_occtCompound, rkOcctShape);
	}

	const TopoDS_Compound& GlobalCluster::GetOcctCompound() const
	{
		return m_occtCompound;
	}
	
	TopoDS_Compound& GlobalCluster::GetOcctCompound()
	{
		return m_occtCompound;
	}

	void GlobalCluster::SubTopologies(std::list<Topology::Ptr>& rSubTopologies) const
	{
		BOPCol_ListOfShape occtListMembers;
		Topology::SubTopologies(GetOcctCompound(), occtListMembers);
		for (BOPCol_ListIteratorOfListOfShape occtIterator(occtListMembers);
			occtIterator.More();
			occtIterator.Next())
		{
			Topology::Ptr pMemberTopology = Topology::ByOcctShape(occtIterator.Value(), "");
			rSubTopologies.push_back(pMemberTopology);
		}
	}

	GlobalCluster::GlobalCluster()
	{
		m_occtBuilder.MakeCompound(m_occtCompound);
	}

	GlobalCluster::~GlobalCluster()
	{
	}
}