#include "TopologyUtility.h"

#include "TopologicCore/include/GlobalCluster.h"
#include "TopologicCore/include/AttributeManager.h"

#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <Geom_Point.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax3.hxx>

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
		BRepBuilderAPI_Transform transform(kpTopology->GetOcctShape(), transformation, true);
		TopologicCore::Topology::Ptr pCoreTransformedTopology = TopologicCore::Topology::ByOcctShape(transform.Shape());

		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCoreTransformedTopology);
		return pCoreTransformedTopology;
	}

	TopologicCore::Topology::Ptr TopologyUtility::Rotate(const TopologicCore::Topology::Ptr & kpTopology,
		const TopologicCore::Vertex::Ptr& kpOrigin,
		const double kDirectionX, const double kDirectionY, const double kDirectionZ,
		const double kDegree)
	{
		gp_Trsf transformation;
		Handle(Geom_Point) pOcctOrigin = kpOrigin->Point();
		double radian = DegreeToRadian(kDegree);
		transformation.SetRotation(gp_Ax1(gp_Pnt(pOcctOrigin->X(), pOcctOrigin->Y(), pOcctOrigin->Z()), gp_Dir(kDirectionX, kDirectionY, kDirectionZ)), radian);
		BRepBuilderAPI_Transform transform(kpTopology->GetOcctShape(), transformation, true);
		TopologicCore::Topology::Ptr pCoreTransformedTopology = TopologicCore::Topology::ByOcctShape(transform.Shape());

		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCoreTransformedTopology);
		return pCoreTransformedTopology;
	}

	TopologicCore::Topology::Ptr TopologyUtility::Transform(const TopologicCore::Topology::Ptr & kpTopology, const TopologicCore::Vertex::Ptr & kpOrigin, const double kNormalX, const double kNormalY, const double kNormalZ, const double kXAxisX, const double kXAxisY, const double kXAxisZ)
	{
		gp_Trsf transformation;
		Handle(Geom_Point) pOcctOrigin = kpOrigin->Point();
		transformation.SetTransformation(
			gp_Ax3(
				gp_Pnt(pOcctOrigin->X(), pOcctOrigin->Y(), pOcctOrigin->Z()), 
				gp_Dir(kNormalX, kNormalY, kNormalZ),
				gp_Dir(kXAxisX, kXAxisY, kXAxisZ)));
		BRepBuilderAPI_Transform transform(kpTopology->GetOcctShape(), transformation, true);
		TopologicCore::Topology::Ptr pCoreTransformedTopology = TopologicCore::Topology::ByOcctShape(transform.Shape());

		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCoreTransformedTopology);
		return pCoreTransformedTopology;
	}

	TopologicCore::Topology::Ptr TopologyUtility::Transform(const TopologicCore::Topology::Ptr & kpTopology, const double kTranslationX, const double kTranslationY, const double kTranslationZ, const double kRotation11, const double kRotation12, const double kRotation13, const double kRotation21, const double kRotation22, const double kRotation23, const double kRotation31, const double kRotation32, const double kRotation33)
	{
		gp_GTrsf gTransformation(
			gp_Mat(kRotation11, kRotation12, kRotation13, kRotation21, kRotation22, kRotation23, kRotation31, kRotation32, kRotation33),
			gp_XYZ(kTranslationX, kTranslationY, kTranslationZ)
		);
		BRepBuilderAPI_GTransform gTransform(kpTopology->GetOcctShape(), gTransformation, true);
		TopologicCore::Topology::Ptr pCoreTransformedTopology = TopologicCore::Topology::ByOcctShape(gTransform.Shape());

		TopologicCore::AttributeManager::GetInstance().CopyAttributes(kpTopology->GetOcctShape(), pCoreTransformedTopology->GetOcctShape());
		TopologicCore::Topology::TransferContents(kpTopology->GetOcctShape(), pCoreTransformedTopology);
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