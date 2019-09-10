#include "..\include\WireUtility.h"

void TopologicUtilities::WireUtility::AdjacentShells(
	const TopologicCore::Wire::Ptr & kpWire, 
	const TopologicCore::Topology::Ptr & kpParentTopology, 
	std::list<TopologicCore::Shell::Ptr>& rCoreAdjacentShells)
{
	std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
	kpWire->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Shell::Type(), coreAdjacentTopologies);
	for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
	{
		rCoreAdjacentShells.push_back(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Shell>(kpAdjacentTopology)
		);
	}
}

void TopologicUtilities::WireUtility::AdjacentCells(const TopologicCore::Wire::Ptr & kpWire, const TopologicCore::Topology::Ptr & kpParentTopology, std::list<TopologicCore::Cell::Ptr>& rCoreAdjacentCells)
{
	std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
	kpWire->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Cell::Type(), coreAdjacentTopologies);
	for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
	{
		rCoreAdjacentCells.push_back(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Cell>(kpAdjacentTopology)
		);
	}
}
