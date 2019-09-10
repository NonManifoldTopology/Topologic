#include "FaceUtility.h"
#include "TopologyUtility.h"

#include <TopologicCore/include/Edge.h>
#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/GlobalCluster.h>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
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
#include <ShapeFix_Edge.hxx>
#include <ShapeFix_Face.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>

#include <BRepCheck_Analyzer.hxx>
#include <BRepCheck_Face.hxx>
#include <ShapeBuild_ReShape.hxx>

namespace TopologicUtilities
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

	void FaceUtility::ParametersAtVertex(const TopologicCore::Face::Ptr& kpFace, const TopologicCore::Vertex::Ptr& kpVertex, double& rU, double& rV)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();
		ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctSurface);
		gp_Pnt2d occtUV = occtSurfaceAnalysis.ValueOfUV(kpVertex->Point()->Pnt(), Precision::Confusion());

		// Non-normalized UV
		double occtU = occtUV.X();
		double occtV = occtUV.Y();

		NormalizeUV(kpFace, occtU, occtV, rU, rV);
	}

	TopologicCore::Vertex::Ptr FaceUtility::VertexAtParameters(const TopologicCore::Face::Ptr& kpFace, const double kU, const double kV)
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

	gp_Dir FaceUtility::NormalAtParameters(const TopologicCore::Face::Ptr& kpFace, const double kU, const double kV)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();
		double occtU = 0.0, occtV = 0.0;
		NonNormalizeUV(kpFace, kU, kV, occtU, occtV);

		GeomLProp_SLProps occtProperties(kpFace->Surface(), occtU, occtV, 1, Precision::Confusion());

		gp_Dir occtNormal = occtProperties.Normal();
		if (kpFace->IsReversed())
		{
			occtNormal.Reverse();
		}
		return occtNormal;

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

	TopologicCore::Face::Ptr TrimByWireImpl(const TopologicCore::Face::Ptr& kpFace, const TopoDS_Wire& rkOcctWire, const bool kReverseWire)
	{
		Handle(Geom_Surface) pOcctSurface = kpFace->Surface();

		ShapeFix_Wire wireFix;
		wireFix.Load(rkOcctWire);
		wireFix.Perform();

		TopoDS_Wire trimmingWire;
		if (kReverseWire)
		{
			trimmingWire = TopoDS::Wire(wireFix.Wire().Reversed());
		}
		else
		{
			trimmingWire = TopoDS::Wire(wireFix.Wire());
		}
		BRepBuilderAPI_MakeFace occtTrimMakeFace(pOcctSurface, trimmingWire);
		if (occtTrimMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			TopologicCore::Face::Throw(occtTrimMakeFace);
		}

		TopoDS_Face coreResultingFace = occtTrimMakeFace;

		ShapeFix_Shape occtFixShape(coreResultingFace);
		occtFixShape.Perform();
		
		// Fix edges
		{
			TopTools_MapOfShape occtEdges;
			for (TopExp_Explorer occtExplorer(coreResultingFace, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				TopoDS_Edge occtEdge = TopoDS::Edge(occtCurrent);
				ShapeFix_Edge occtFixEdge;
				bool result1 = occtFixEdge.FixAddCurve3d(occtEdge);
				bool result2 = occtFixEdge.FixVertexTolerance(occtEdge);
			}
		}

		// Fix wires
		{
			TopTools_MapOfShape occtWires;
			for (TopExp_Explorer occtExplorer(coreResultingFace, TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
			{
				const TopoDS_Shape& occtCurrent = occtExplorer.Current();
				const TopoDS_Wire& occtWire = TopoDS::Wire(occtCurrent);
				ShapeFix_Wire occtFixWire(occtWire, coreResultingFace, 0.0001);
				bool result1 = occtFixWire.Perform();
			}
		}

		ShapeFix_Face faceFix(coreResultingFace);
		int result = faceFix.Perform();

		Handle(ShapeBuild_ReShape) occtContext = new ShapeBuild_ReShape();
		occtContext->Apply(faceFix.Face());

		TopoDS_Face occtFinalFace = TopoDS::Face(ShapeFix::RemoveSmallEdges(coreResultingFace, 0.0001, occtContext));

#ifdef _DEBUG
		BRepCheck_Analyzer occtAnalyzer(occtFinalFace);
		bool isValid = occtAnalyzer.IsValid();

		BRepCheck_Face occtFaceCheck(TopoDS::Face(occtFinalFace));
		bool isUnorientable = occtFaceCheck.IsUnorientable();
		BRepCheck_Status orientationStatus = occtFaceCheck.OrientationOfWires();
		BRepCheck_Status intersectionStatus = occtFaceCheck.IntersectWires();
		BRepCheck_Status classificationStatus = occtFaceCheck.ClassifyWires();
#endif

		return std::make_shared<TopologicCore::Face>(TopoDS::Face(occtFinalFace));
	}

	TopologicCore::Face::Ptr FaceUtility::TrimByWire(const TopologicCore::Face::Ptr& kpFace, const TopologicCore::Wire::Ptr& kpWire, const bool kReverseWire)
	{
		TopologicCore::Face::Ptr pCopyInputFace = std::dynamic_pointer_cast<TopologicCore::Face>(kpFace->ShallowCopy());
		TopologicCore::Wire::Ptr pCopyInputWire = std::dynamic_pointer_cast<TopologicCore::Wire>(kpWire->ShallowCopy());
		TopologicCore::Face::Ptr pFace = TrimByWireImpl(pCopyInputFace, pCopyInputWire->GetOcctWire(), kReverseWire);
		TopologicCore::Face::Ptr pCopyOutputFace = std::dynamic_pointer_cast<TopologicCore::Face>(pFace->DeepCopy());
		TopologicCore::GlobalCluster::GetInstance().AddTopology(pCopyOutputFace->GetOcctFace());
		return pCopyOutputFace;
	}

	void FaceUtility::Triangulate(const TopologicCore::Face::Ptr & kpFace, const double kDeflection, std::list<TopologicCore::Face::Ptr>& rTriangles)
	{
		TopoDS_Face occtFace = kpFace->GetOcctFace();
		BRepMesh_IncrementalMesh occtIncrementalMesh(occtFace, kDeflection);
		TopLoc_Location occtLocation;
		Handle(Poly_Triangulation) pOcctTriangulation = BRep_Tool::Triangulation(occtFace, occtLocation);
		if (pOcctTriangulation.IsNull())
		{
			throw std::exception("No triangulation was produced.");
		}
		int numOfTriangles = pOcctTriangulation->NbTriangles();
		for (int i = 1; i <= numOfTriangles; ++i)
		{
			int index1 = 0, index2 = 0, index3 = 0;
			pOcctTriangulation->Triangle(i).Get(index1, index2, index3);
			gp_Pnt point1 = pOcctTriangulation->Node(index1);
			gp_Pnt point2 = pOcctTriangulation->Node(index2);
			gp_Pnt point3 = pOcctTriangulation->Node(index3);

			TopologicCore::Vertex::Ptr vertex1 = TopologicCore::Vertex::ByPoint(new Geom_CartesianPoint(point1));
			TopologicCore::Vertex::Ptr vertex2 = TopologicCore::Vertex::ByPoint(new Geom_CartesianPoint(point2));
			TopologicCore::Vertex::Ptr vertex3 = TopologicCore::Vertex::ByPoint(new Geom_CartesianPoint(point3));

			TopologicCore::Edge::Ptr edge1 = TopologicCore::Edge::ByStartVertexEndVertex(vertex1, vertex2);
			TopologicCore::Edge::Ptr edge2 = TopologicCore::Edge::ByStartVertexEndVertex(vertex2, vertex3);
			TopologicCore::Edge::Ptr edge3 = TopologicCore::Edge::ByStartVertexEndVertex(vertex3, vertex1);
			std::list<TopologicCore::Edge::Ptr> edges;
			edges.push_back(edge1);
			edges.push_back(edge2);
			edges.push_back(edge3);

			TopologicCore::Face::Ptr face = TopologicCore::Face::ByEdges(edges);
			rTriangles.push_back(face);
		}
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
	
	void FaceUtility::AdjacentShells(const TopologicCore::Face::Ptr & kpFace, const TopologicCore::Topology::Ptr & kpParentTopology, std::list<TopologicCore::Shell::Ptr>& rCoreAdjacentShells)
	{
		std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
		kpFace->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Shell::Type(), coreAdjacentTopologies);
		for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
		{
			rCoreAdjacentShells.push_back(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Shell>(kpAdjacentTopology)
			);
		}
	}
	
	void FaceUtility::AdjacentCells(const TopologicCore::Face::Ptr & kpFace, const TopologicCore::Topology::Ptr & kpParentTopology, std::list<TopologicCore::Cell::Ptr>& rCoreAdjacentCells)
	{
		std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
		kpFace->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Cell::Type(), coreAdjacentTopologies);
		for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
		{
			rCoreAdjacentCells.push_back(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Cell>(kpAdjacentTopology)
			);
		}
	}
}