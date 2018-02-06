#include <Cluster.h>
#include <GlobalCluster.h>

#include <TopoDS_Builder.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>
#include <TopoDS_FrozenShape.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	Cluster* Cluster::ByTopology(const std::list<Topology*>& rkTopologies)
	{
		Cluster* pCluster = new Cluster(new TopoDS_Compound());
		for(std::list<Topology*>::const_iterator kTopologyIterator = rkTopologies.begin();
			kTopologyIterator != rkTopologies.end();
			kTopologyIterator++)
		{
			pCluster->Add(*kTopologyIterator);
		}
		return pCluster;
	}

	bool Cluster::Add(Topology const * const kpkTopology)
	{
		try {
			TopoDS_Builder occtBuilder;
			occtBuilder.Add(*GetOcctShape(), *kpkTopology->GetOcctShape());

			return true;
		}
		catch (TopoDS_UnCompatibleShapes &)
		{
			return false;
		}
		catch (TopoDS_FrozenShape &)
		{
			return false;
		}
	}

	bool Cluster::Remove(Topology const * const kpkTopology)
	{
		try {
			TopoDS_Builder occtBuilder;
			occtBuilder.Remove(*GetOcctShape(), *kpkTopology->GetOcctShape());

			return true;
		}
		catch (TopoDS_UnCompatibleShapes &)
		{
			return false;
		}
		catch (TopoDS_FrozenShape &)
		{
			return false;
		}
	}

	TopoDS_Shape* Cluster::GetOcctShape() const
	{
		assert(m_pOcctCompound != nullptr && "Cluster::m_pOcctCompound is null.");
		if (m_pOcctCompound == nullptr)
		{
			throw std::exception("Cluster::m_pOcctCompound is null.");
		}

		return m_pOcctCompound;
	}

	void Cluster::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		throw std::exception("No implementation for Cluster entity");
	}

	Cluster::Cluster(TopoDS_Compound * const kpOcctCompound, const bool kAddToGlobalCluster)
		: Topology(3)
		, m_pOcctCompound(kpOcctCompound)
	{
		TopoDS_Builder occtBuilder;
		occtBuilder.MakeCompound(*m_pOcctCompound);

		if (kAddToGlobalCluster)
		{
			GlobalCluster::GetInstance().Add(this);
		}
	}

	Cluster::~Cluster()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctCompound;
	}
}
