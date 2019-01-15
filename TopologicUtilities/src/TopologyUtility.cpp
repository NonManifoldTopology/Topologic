#include "TopologyUtility.h"

#include <BRepExtrema_DistShapeShape.hxx>

namespace TopologicUtilities
{
	double TopologyUtility::Distance(const TopologicCore::Topology::Ptr& kpTopology, const TopologicCore::Topology::Ptr& kpAnotherTopology)
	{
		BRepExtrema_DistShapeShape occtDistance(kpTopology->GetOcctShape(), kpAnotherTopology->GetOcctShape());
		return occtDistance.Value();
	}

	void TopologyUtility::Translate(const TopologicCore::Topology::Ptr& kpTopology, const double x, const double y, const double z)
	{
		gp_Trsf transformation;
		transformation.SetTranslation(gp_Vec(x, y, z));
		kpTopology->GetOcctShape().Move(TopLoc_Location(transformation));
	}

}