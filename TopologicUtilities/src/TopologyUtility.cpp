#include "TopologyUtility.h"

#include "TopologicCore/include/GlobalCluster.h"

#include <BRepBuilderAPI_GTransform.hxx>
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

	TopologicCore::Topology::Ptr TopologyUtility::Translate(const TopologicCore::Topology::Ptr& kpTopology, const double x, const double y, const double z)
	{
		gp_Trsf transformation;
		transformation.SetTranslation(gp_Vec(x, y, z));
		TopologicCore::Topology::Ptr pCoreTransformedTopology = kpTopology->DeepCopy();
		pCoreTransformedTopology->GetOcctShape().Move(TopLoc_Location(transformation));
		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCoreTransformedTopology);
		return pCoreTransformedTopology;
	}

	TopologicCore::Topology::Ptr TopologyUtility::Rotate(const TopologicCore::Topology::Ptr & kpTopology,
		const TopologicCore::Vertex::Ptr& kpOrigin,
		const double kDirectionX, const double kDirectionY, const double kDirectionZ,
		const double kDegree)
	{
		Handle(Geom_Point) pOcctOrigin = kpOrigin->Point();
		double radian = DegreeToRadian(kDegree);
		gp_Trsf transformation;
		transformation.SetRotation(gp_Ax1(gp_Pnt(pOcctOrigin->X(), pOcctOrigin->Y(), pOcctOrigin->Z()), gp_Dir(kDirectionX, kDirectionY, kDirectionZ)), radian);
		TopologicCore::Topology::Ptr pCoreTransformedTopology = kpTopology->DeepCopy();
		pCoreTransformedTopology->GetOcctShape().Move(TopLoc_Location(transformation)); 
		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCoreTransformedTopology);
		return pCoreTransformedTopology;
	}

	TopologicCore::Topology::Ptr TopologyUtility::Scale(
		const TopologicCore::Topology::Ptr & kpTopology, const TopologicCore::Vertex::Ptr & kpOrigin,
		const double kXFactor, const double kYFactor, const double kZFactor)
	{
		Handle(Geom_Point) pOcctOrigin = kpOrigin->Point();
		gp_GTrsf occtGTransformation(
			gp_Mat(
				kXFactor, 0.0, 0.0,
				0.0, kYFactor, 0.0,
				0.0, 0.0, kZFactor),
			gp_XYZ(pOcctOrigin->X(), pOcctOrigin->Y(), pOcctOrigin->Z())
		);
		BRepBuilderAPI_GTransform occtTransform(kpTopology->DeepCopy()->GetOcctShape(), occtGTransformation);
		TopoDS_Shape occtTransformedShape = occtTransform.Shape();
		TopologicCore::Topology::Ptr pCoreTransformedTopology = TopologicCore::Topology::ByOcctShape(occtTransformedShape, kpTopology->GetInstanceGUID());
		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCoreTransformedTopology);
		return pCoreTransformedTopology;
	}

	double TopologyUtility::DegreeToRadian(const double kDegree)
	{
		return kDegree * M_PI / 180.0;
	}

	double TopologyUtility::RadianToDegree(const double kRadian)
	{
		return kRadian * 180.0 / M_PI;
	}
}