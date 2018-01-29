#include <Topology.h>


namespace TopoLogicCore
{
	Topology::Topology(const int kDimensionality)
		: m_dimensionality(kDimensionality)
		, m_isLocked(false)
		, m_modelingPriority(5)
	{

	}

	Topology::~Topology()
	{

		for (AttributeMapIterator attributeMapIterator = m_attributeMap.begin();
			attributeMapIterator != m_attributeMap.end();
			attributeMapIterator++)
		{
			delete attributeMapIterator->second;
		}
		m_attributeMap.clear();

		for (std::list<Topology*>::iterator memberIterator = m_members.begin();
			memberIterator != m_members.end();
			memberIterator++)
		{
			delete *memberIterator;
		}
		m_members.clear();


		for (std::list<Topology*>::iterator memberOfIterator = m_membersOf.begin();
			memberOfIterator != m_membersOf.end();
			memberOfIterator++)
		{
			delete *memberOfIterator;
		}
		m_membersOf.clear();
	}

	Topology* Topology::Difference(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
	Topology* Topology::Impose(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
	Topology* Topology::Imprint(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
	Topology* Topology::Intersection(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
	Topology* Topology::Merge(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
	Topology* Topology::Slice(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
	Topology* Topology::Union(Topology const * const kpkTopology1, Topology const * const kpkTopology2)
	{
		return nullptr;
	}
}