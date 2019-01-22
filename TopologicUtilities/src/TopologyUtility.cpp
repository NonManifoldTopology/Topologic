#include "TopologyUtility.h"

#include <BRepExtrema_DistShapeShape.hxx>
#include <Geom_Point.hxx>
#include <gp_Ax1.hxx>

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

	void TopologyUtility::Rotate(const TopologicCore::Topology::Ptr & kpTopology,
		const double kPointX, const double kPointY, const double kPointZ,
		const double kDirectionX, const double kDirectionY, const double kDirectionZ,
		const double kAngleInRadian)
	{
		gp_Trsf transformation;
		transformation.SetRotation(gp_Ax1(gp_Pnt(kPointX, kPointY, kPointZ), gp_Dir(kDirectionX, kDirectionY, kDirectionZ)), kAngleInRadian);
		kpTopology->GetOcctShape().Move(TopLoc_Location(transformation));
	}

	void TopologyUtility::Scale(const TopologicCore::Topology::Ptr & kpTopology, const TopologicCore::Vertex::Ptr & kpVertex, const double kScaleFactor)
	{
		gp_Trsf transformation;
		transformation.SetScale(kpVertex->Point()->Pnt(), kScaleFactor);
		kpTopology->GetOcctShape().Move(TopLoc_Location(transformation));
	}

}