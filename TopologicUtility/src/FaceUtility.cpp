#include "FaceUtility.h"
#include "TopologyUtility.h"

#include <TopologicCore/include/Vertex.h>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepGProp.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <GeomConvert.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <ShapeFix_Face.hxx>
#include <TopoDS.hxx>

namespace TopologicUtility
{
	double FaceUtility::Area(const TopologicCore::Face::Ptr& kpFace)
	{
		GProp_GProps occtShapeProperties;
		ShapeFix_Face occtShapeFix(kpFace->GetOcctFace());
		occtShapeFix.Perform();
		BRepGProp::SurfaceProperties(occtShapeFix.Face(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

	TopologicCore::Face::Ptr FaceUtility::ByVertices(const std::list<std::list<TopologicCore::Vertex::Ptr>>& rkVertices)
	{
		// Check if all vertex lists have the same length
		int rowLength = 0;
		bool firstRow = true;
		for (const std::list<TopologicCore::Vertex::Ptr>& rkVerticesRow : rkVertices)
		{
			if (firstRow)
			{
				rowLength = (int)rkVerticesRow.size();
				firstRow = false;
			}
			else
			{
				if (rowLength != rkVerticesRow.size())
				{
					throw std::exception("Rows have inequal lengths");
				}
			}
		}
		int columnLength = (int)rkVertices.size();

		TColgp_Array2OfPnt occtPoints(1, columnLength, 1, rowLength);
		int i = 1;
		for (const std::list<TopologicCore::Vertex::Ptr>& rkVerticesRow : rkVertices)
		{
			int j = 1;
			for (const TopologicCore::Vertex::Ptr& kpVertex : rkVerticesRow)
			{
				occtPoints.SetValue(i, j, kpVertex->Point()->Pnt());
				j++;
			}
			i++;
		}
		try {
			Handle(Geom_Surface) pSurface = GeomAPI_PointsToBSplineSurface(occtPoints).Surface();
			return TopologicCore::Face::BySurface(pSurface);
		}
		catch (...)
		{
			throw std::exception("Fails to create a face by vertices");
		}
	}

	void FaceUtility::UVParameterAtVertex(const TopologicCore::Face::Ptr& kpFace, const TopologicCore::Vertex::Ptr& kpVertex, double& rU, double& rV)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();
		ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctSurface);
		gp_Pnt2d occtUV = occtSurfaceAnalysis.ValueOfUV(kpVertex->Point()->Pnt(), Precision::Confusion());

		// Non-normalized UV
		double occtU = occtUV.X();
		double occtV = occtUV.Y();

		NormalizeUV(kpFace, occtU, occtV, rU, rV);
	}

	TopologicCore::Vertex::Ptr FaceUtility::VertexAtParameter(const TopologicCore::Face::Ptr& kpFace, const double kU, const double kV)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();
		double occtU = 0.0, occtV = 0.0;
		NonNormalizeUV(kpFace, kU, kV, occtU, occtV);

		double occtMinU = 0.0, occtMaxU = 0.0, occtMinV = 0.0, occtMaxV = 0.0;
		ShapeAnalysis::GetFaceUVBounds(kpFace->GetOcctFace(), occtMinU, occtMaxU, occtMinV, occtMaxV);
		ShapeAnalysis_Surface occtSurfaceAnalysis(kpFace->Surface());
		const Handle(Geom_Surface)& rSurface = occtSurfaceAnalysis.Surface();
		Handle(Geom_RectangularTrimmedSurface) pTrimmedSurface = new Geom_RectangularTrimmedSurface(rSurface, occtMinU, occtMaxU, occtMinV, occtMaxV);
		Handle(Geom_BSplineSurface) pBSplineSurface = GeomConvert::SurfaceToBSplineSurface(pTrimmedSurface);
		//gp_Pnt occtPoint = occtSurfaceAnalysis.Value(occtU, occtV);
		gp_Pnt occtPoint = pBSplineSurface->Value(occtU, occtV);
		TopologicCore::Vertex::Ptr vertex = TopologicCore::Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));

		// Check distance: if more than a small aount, it is not part of the face.
		// Hack: project to surface
		double distance = TopologyUtility::Distance(kpFace, vertex);
		if (distance > Precision::Confusion())
		{
			throw std::exception("Point not on the face.");
		}

		return TopologicCore::Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));
	}

	gp_Dir FaceUtility::NormalAtParameter(const TopologicCore::Face::Ptr& kpFace, const double kU, const double kV)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();
		double occtU = 0.0, occtV = 0.0;
		NonNormalizeUV(kpFace, kU, kV, occtU, occtV);

		GeomLProp_SLProps occtProperties(kpFace->Surface(), occtU, occtV, 1, Precision::Confusion());

		return occtProperties.Normal();
	}

	void FaceUtility::UVSamplePoints(
		const TopologicCore::Face::Ptr& kpFace,
		const std::list<double>& rkUValues,
		const std::list<double>& rkVValues,
		std::list<std::list<gp_Pnt>>& rSamplesPoints,
		std::list<double>& rOcctUValues,
		std::list<double>& rOcctVValues,
		int& rNumUPoints,
		int& rNumVPoints,
		int& rNumUPanels,
		int& rNumVPanels,
		bool& rIsUClosed,
		bool& rIsVClosed)
		/*,
		std::list<double>& rOcctUValues,
		std::list<double>& rOcctVValues)*/
	{
		// Subdivide the face in the UV space and find the points
		double occtUMin = 0.0, occtUMax = 0.0, occtVMin = 0.0, occtVMax = 0.0;
		ShapeFix_Face occtShapeFix(kpFace->GetOcctFace());
		occtShapeFix.Perform();
		ShapeAnalysis::GetFaceUVBounds(occtShapeFix.Face(), occtUMin, occtUMax, occtVMin, occtVMax);
		double occtURange = occtUMax - occtUMin;
		double occtVRange = occtVMax - occtVMin;
		rNumUPanels = (int)rkUValues.size() - 1;
		rNumVPanels = (int)rkVValues.size() - 1;

		Handle(Geom_Surface) pOcctWallSurface = kpFace->Surface();
		rIsUClosed = pOcctWallSurface->IsUClosed();
		rNumUPoints = rNumUPanels;
		if (!rIsUClosed)
		{
			rNumUPoints += 1;
		}
		rIsVClosed = pOcctWallSurface->IsVClosed();
		rNumVPoints = rNumVPanels;
		if (!rIsVClosed)
		{
			rNumVPoints += 1;
		}

		// Compute OCCT's non-normalized UV values
		// At the same time, get the isolines
		BOPCol_ListOfShape occtIsolines;
		//std::list<double> occtUValues;
		for (double u : rkUValues)
		{
			double occtU = occtUMin + occtURange * u;
			if (occtU < occtUMin) {
				occtU = occtUMin;
			}
			else if (occtU > occtUMax)
			{
				occtU = occtUMax;
			}
			rOcctUValues.push_back(occtU);
		};
		//std::list<double> occtVValues;
		for (double v : rkVValues)
		{
			double occtV = occtVMin + occtVRange * v;
			if (occtV < occtVMin) {
				occtV = occtVMin;
			}
			else if (occtV > occtVMax)
			{
				occtV = occtVMax;
			}
			rOcctVValues.push_back(occtV);
		};

		// Sample the points
		int numOfPoints = rNumUPoints * rNumVPoints;
		int i = 0;
		std::list<double>::const_iterator endUIterator = rOcctUValues.end();
		if (rIsUClosed)
		{
			endUIterator--;
		}
		std::list<double>::const_iterator endVIterator = rOcctVValues.end();
		if (rIsVClosed)
		{
			endVIterator--;
		}

		for (std::list<double>::const_iterator uIterator = rOcctUValues.begin();
			uIterator != endUIterator;
			uIterator++)
		{
			std::list<gp_Pnt> rowSamplePoints;
			const double& rkU = *uIterator;
			for (std::list<double>::const_iterator vIterator = rOcctVValues.begin();
				vIterator != endVIterator;
				vIterator++)
			{
				const double& rkV = *vIterator;
				gp_Pnt occtPoint = pOcctWallSurface->Value(rkU, rkV);
				rowSamplePoints.push_back(occtPoint);
			}
			rSamplesPoints.push_back(rowSamplePoints);
		}
	}

	TopologicCore::Face::Ptr FaceUtility::TrimByWire(const TopologicCore::Face::Ptr& kpFace, const TopologicCore::Wire::Ptr& kpWire)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();
		const TopoDS_Wire& rkWire = kpWire->GetOcctWire();

		BRepBuilderAPI_MakeFace occtTrimMakeFace(pOcctSurface, rkWire);
		if (occtTrimMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			TopologicCore::Face::Throw(occtTrimMakeFace);
		}
		return std::make_shared<TopologicCore::Face>(TopoDS::Face(occtTrimMakeFace.Shape()));
	}

	void FaceUtility::NormalizeUV(const TopologicCore::Face::Ptr& kpFace, const double kNonNormalizedU, const double kNonNormalizedV, double& rNormalizedU, double& rNormalizedV)
	{
		double occtUMin = 0.0, occtUMax = 0.0, occtVMin = 0.0, occtVMax = 0.0;
		ShapeFix_Face occtShapeFix(kpFace->GetOcctFace());
		occtShapeFix.Perform();
		ShapeAnalysis::GetFaceUVBounds(occtShapeFix.Face(), occtUMin, occtUMax, occtVMin, occtVMax);
		double occtDU = occtUMax - occtUMin;
		double occtDV = occtVMax - occtVMin;
		if (occtDU <= 0.0 || occtDV <= 0.0)
		{
			throw std::exception("Negative range");
		}

		rNormalizedU = (kNonNormalizedU - occtUMin) / occtDU;
		rNormalizedV = (kNonNormalizedV - occtVMin) / occtDV;
	}

	void FaceUtility::NonNormalizeUV(const TopologicCore::Face::Ptr& kpFace, const double kNormalizedU, const double kNormalizedV, double& rNonNormalizedU, double& rNonNormalizedV)
	{
		double occtUMin = 0.0, occtUMax = 0.0, occtVMin = 0.0, occtVMax = 0.0;
		ShapeFix_Face occtShapeFix(kpFace->GetOcctFace());
		occtShapeFix.Perform();
		ShapeAnalysis::GetFaceUVBounds(occtShapeFix.Face(), occtUMin, occtUMax, occtVMin, occtVMax);
		double occtDU = occtUMax - occtUMin;
		double occtDV = occtVMax - occtVMin;

		rNonNormalizedU = occtUMin + kNormalizedU * occtDU;
		rNonNormalizedV = occtVMin + kNormalizedV * occtDV;
	}
}