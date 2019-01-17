#include "EdgeUtility.h"

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepGProp.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Line.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomLib_Tool.hxx>
#include <GProp_GProps.hxx>
#include <Precision.hxx>

namespace TopologicUtilities
{
	TopologicCore::Edge::Ptr EdgeUtility::ByVertices(const std::list<TopologicCore::Vertex::Ptr>& rkVertices)
	{
		// Cases:
		// - 0 or 1 vertex --> invalid
		// - 2 --> line
		// - 3 --> BSpline

		int numberOfVertices = (int)rkVertices.size();
		TopologicCore::Edge::Ptr pEdge = nullptr;
		if (numberOfVertices < 2)
		{
			throw std::exception("Too few vertices to create an edge.");
		}
		else if (numberOfVertices == 2) // a line
		{
			const TopologicCore::Vertex::Ptr& rkVertex1 = *(rkVertices.begin());
			const TopologicCore::Vertex::Ptr& rkVertex2 = *(++rkVertices.begin());
			return TopologicCore::Edge::ByStartVertexEndVertex(rkVertex1, rkVertex2);

		}
		else
		{
			// else more than 2 vertices
			Handle(TColgp_HArray1OfPnt) pOcctPoints = new TColgp_HArray1OfPnt(1, numberOfVertices);
			int i = 1;
			for (const TopologicCore::Vertex::Ptr& kpVertex : rkVertices)
			{
				pOcctPoints->SetValue(i, kpVertex->Point()->Pnt());
				++i;
			}

			try {
				GeomAPI_Interpolate occtInterpolate(pOcctPoints, false, Precision::Confusion());
				occtInterpolate.Perform();
				if (!occtInterpolate.IsDone())
				{
					throw std::exception("Line interpolation error in Edge::ByVertices()");
				}
				Handle(Geom_Curve) pOcctCurveOnTargetSurface = occtInterpolate.Curve();
				BRepBuilderAPI_MakeEdge occtMakeEdge(pOcctCurveOnTargetSurface);
				if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
				{
					TopologicCore::Edge::Throw(occtMakeEdge.Error());
				}

				pEdge = std::make_shared<TopologicCore::Edge>(occtMakeEdge);
			}
			catch (Standard_Failure e)
			{
				throw std::exception(e.GetMessageString());
			}
		}

		return pEdge;
	}

	TopologicCore::Edge::Ptr EdgeUtility::ByCircle(
		const std::shared_ptr<TopologicCore::Vertex>& kpCenterPoint, const double kRadius,
		const double kXAxisX, const double kXAxisY, const double kXAxisZ,
		const double kNormalX, const double kNormalY, const double kNormalZ)
	{
		Handle(Geom_Circle) pOcctCircle = new Geom_Circle(
			gp_Ax2(
				kpCenterPoint->Point()->Pnt(),
				gp_Dir(kNormalX, kNormalY, kNormalZ),
				gp_Dir(kXAxisX, kXAxisY, kXAxisZ)
			),
			kRadius
		);
		return TopologicCore::Edge::ByCurve(pOcctCircle);
	}

	double EdgeUtility::ParameterAtPoint(const TopologicCore::Edge::Ptr& kpEdge, const TopologicCore::Vertex::Ptr& kpVertex)
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = kpEdge->Curve(u0, u1);
		Handle(Geom_Point) pOcctPoint = kpVertex->Point();

		double occtParameter = 0.0;
		bool isOnCurve = GeomLib_Tool::Parameter(pOcctCurve, pOcctPoint->Pnt(), Precision::Confusion(), occtParameter);
		if (!isOnCurve)
		{
			throw std::exception("Point not on curve");
		}

		// Parameter may be non-normalized, so normalize it
		return kpEdge->NormalizeParameter(u0, u1, occtParameter);
	}

	TopologicCore::Vertex::Ptr EdgeUtility::PointAtParameter(const TopologicCore::Edge::Ptr& kpEdge, const double kParameter)
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = kpEdge->Curve(u0, u1);
		Handle(Geom_Line) pOcctLine = Handle(Geom_Line)::DownCast(pOcctCurve);
		if (!pOcctLine.IsNull())
		{
			u0 = 0.0;
			u1 = Length(kpEdge);
		}

		// Parameter is normalized, so non-normalize it
		double occtParameter = kpEdge->NonNormalizeParameter(u0, u1, kParameter);
		gp_Pnt occtPoint = pOcctCurve->Value(occtParameter);

		return TopologicCore::Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));
	}

	double EdgeUtility::Length(const TopologicCore::Edge::Ptr& kpEdge)
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::LinearProperties(kpEdge->GetOcctShape(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

}