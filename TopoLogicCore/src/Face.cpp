#include <Face.h>
#include <Edge.h>
#include <GlobalCluster.h>
#include <Cell.h>
#include <Shell.h>
#include <Vertex.h>
#include <Wire.h>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BSplCLib.hxx>
#include <BRepGProp.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <ShapeFix_Face.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopOpeBRepBuild_Tools.hxx>
#include <TopTools_MapOfShape.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void Face::AdjacentFaces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		// Get the map of edge->faces
		TopTools_IndexedDataMapOfShapeListOfShape occtEdgeFaceMap;
		TopExp::MapShapesAndUniqueAncestors(GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_EDGE, TopAbs_FACE, occtEdgeFaceMap);

		// Get the wires of this face
		TopTools_MapOfShape occtWires;
		for (TopExp_Explorer occtExplorer(GetOcctShape(), TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtWires.Contains(occtCurrent))
			{
				occtWires.Add(occtCurrent);
			}
		}

		for(TopTools_MapIteratorOfMapOfShape kOcctMapIterator(occtWires);
			kOcctMapIterator.More();
			kOcctMapIterator.Next())
		{
			for (BRepTools_WireExplorer occtWireExplorer(TopoDS::Wire(kOcctMapIterator.Value())); occtWireExplorer.More(); occtWireExplorer.Next())
			{
				TopoDS_Face occtFace;
				bool faceFound = TopOpeBRepBuild_Tools::GetAdjacentFace(GetOcctShape(), occtWireExplorer.Current(), occtEdgeFaceMap, occtFace);
				if (faceFound)
				{
					rFaces.push_back(std::make_shared<Face>(occtFace));
				}
			}
		}
	}

	void Face::Cells(std::list<std::shared_ptr<Cell>>& rCells) const
	{
		UpwardNavigation(rCells);
	}

	void Face::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Face::Shells(std::list<std::shared_ptr<Shell>>& rShells) const
	{
		UpwardNavigation(rShells);
	}

	void Face::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Face::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	double Face::Area() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::SurfaceProperties(GetOcctShape(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

	std::shared_ptr<Face> Face::ByClosedWire(const std::shared_ptr<Wire>& pkWire)
	{
		BRepBuilderAPI_MakeFace occtMakeFace(pkWire->GetOcctWire());
		if (occtMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			Throw(occtMakeFace);
		}

		std::shared_ptr<Face> pFace = std::make_shared<Face>(occtMakeFace);
		//pkWire->AddIngredientTo(pFace);
		return pFace;
	}

	std::shared_ptr<Face> Face::ByEdges(const std::list<std::shared_ptr<Edge>>& rkEdges)
	{
		if (rkEdges.size())
		{
			throw std::exception("No edge is passed.");
		}
		
		std::shared_ptr<Wire> pWire = Wire::ByEdges(rkEdges);
		std::shared_ptr<Face> pFace = ByClosedWire(pWire);
		/*for (std::list<std::shared_ptr<Edge>>::const_iterator kEdgeIterator = rkEdges.begin();
			kEdgeIterator != rkEdges.end();
			kEdgeIterator++)
		{
			const std::shared_ptr<Edge>& kpEdge = *kEdgeIterator;
			kpEdge->AddIngredientTo(pFace);
		}*/

		return pFace;
	}

	std::shared_ptr<Face> Face::BySurface(Handle(Geom_Surface) pOcctSurface)
	{
		BRepBuilderAPI_MakeFace occtMakeFace;
		try {
			occtMakeFace = BRepBuilderAPI_MakeFace(pOcctSurface, Precision::Confusion());
		}
		catch (StdFail_NotDone&)
		{
			Throw(occtMakeFace);
		}
		return std::make_shared<Face>(occtMakeFace);
	}

	std::shared_ptr<Face> Face::BySurface(
		const TColgp_Array2OfPnt& rkOcctPoles, 
		const TColStd_Array2OfReal& rkOcctWeights, 
		const TColStd_Array1OfReal& rkOcctUKnots, 
		const TColStd_Array1OfReal& rkOcctVKnots, 
		const TColStd_Array1OfInteger& rkOcctUMultiplicities, 
		const TColStd_Array1OfInteger& rkOcctVMultiplicities, 
		const int kUDegree, 
		const int kVDegree, 
		const bool kIsUPeriodic, 
		const bool kIsVPeriodic, 
		const bool kIsRational,
		const std::shared_ptr<Wire>& kpOuterWire,
		const std::list<std::shared_ptr<Wire>>& rkInnerWires)
	{
		BRepBuilderAPI_MakeFace occtMakeFace;
		try {
			Handle(Geom_BSplineSurface) pOcctBSplineSurface = new Geom_BSplineSurface(
				rkOcctPoles, 
				rkOcctWeights, 
				rkOcctUKnots, 
				rkOcctVKnots, 
				rkOcctUMultiplicities, 
				rkOcctVMultiplicities, 
				kUDegree, kVDegree, 
				kIsUPeriodic, kIsVPeriodic);
			if (kpOuterWire != nullptr)
			{
				occtMakeFace = BRepBuilderAPI_MakeFace(pOcctBSplineSurface, TopoDS::Wire(kpOuterWire->GetOcctShape().Reversed()), true);
			}
			else
			{
				occtMakeFace = BRepBuilderAPI_MakeFace(pOcctBSplineSurface, Precision::Confusion());
			}
		}
		catch (const Standard_ConstructionError&) // from Geom_BSplineCurve's constructor
		{
			int occtMaxDegree = Geom_BSplineSurface::MaxDegree();
			if (kUDegree < 1 || kUDegree > occtMaxDegree) {
				throw std::exception(std::string("The curve's u degree must be between 1 and " + std::to_string(occtMaxDegree) + " (OCCT's max degree).").c_str());
			}
			if (kVDegree < 1 || kVDegree > occtMaxDegree) {
				throw std::exception(std::string("The curve's v degree must be between 1 and " + std::to_string(occtMaxDegree) + " (OCCT's max degree).").c_str());
			}

			if (rkOcctPoles.ColLength() < 2 || rkOcctPoles.RowLength() < 2)
			{
				throw std::exception("There must be more than one control points along each axis.");
			}

			if (rkOcctPoles.ColLength() != rkOcctWeights.ColLength() || rkOcctPoles.RowLength() != rkOcctWeights.RowLength())
			{
				throw std::exception("The lists of control points and weights must have the same numbers of rows and columns.");
			}

			if (rkOcctUKnots.Length() != rkOcctUMultiplicities.Length() || rkOcctVKnots.Length() != rkOcctVMultiplicities.Length())
			{
				throw std::exception("The lists of U and V knots and multiplicities must have the same length.");
			}

			for (int i = rkOcctWeights.LowerRow(); i <= rkOcctWeights.UpperRow(); i++)
			{
				for (int j = rkOcctWeights.LowerCol(); j <= rkOcctWeights.UpperCol(); j++)
				{
					if (rkOcctWeights(i, j) <= gp::Resolution())
					{
						throw std::exception(std::string("The (" + std::to_string(i) + ", " + std::to_string(j) + ")'s weight is smaller than OCCT's resolution (i.e. practically zero).").c_str());
					}
				}
			}

			for (int i = rkOcctUKnots.Lower(); i < rkOcctUKnots.Upper(); i++)
			{
				if (rkOcctUKnots(i + 1) - rkOcctUKnots(i) <= Epsilon(Abs(rkOcctUKnots(i)))) {
					throw std::exception(std::string("The " + std::to_string(i + 1) + "'s U knot must be larger than the " + std::to_string(i) + "'s U knot.").c_str());
				}
			}
			for (int i = rkOcctVKnots.Lower(); i < rkOcctVKnots.Upper(); i++)
			{
				if (rkOcctVKnots(i + 1) - rkOcctVKnots(i) <= Epsilon(Abs(rkOcctVKnots(i)))) {
					throw std::exception(std::string("The " + std::to_string(i + 1) + "'s V knot must be larger than the " + std::to_string(i) + "'s V knot.").c_str());
				}
			}

			if (rkOcctPoles.Length() != BSplCLib::NbPoles(kUDegree, kIsUPeriodic, rkOcctUMultiplicities))
			{
				if (kIsUPeriodic)
				{
					throw std::exception("The curve is U periodic, so the number of poles must be equal to the sum of the U multiplicities.");
				}
				else
				{
					throw std::exception("The curve is not U periodic, so the number of poles must be equal to the sum of the U multiplicities - U degree - 1, which must be larger than 1.");
				}
			}
			if (rkOcctPoles.Length() != BSplCLib::NbPoles(kVDegree, kIsVPeriodic, rkOcctUMultiplicities))
			{
				if (kIsVPeriodic)
				{
					throw std::exception("The curve is V periodic, so the number of poles must be equal to the sum of the V multiplicities.");
				}
				else
				{
					throw std::exception("The curve is not V periodic, so the number of poles must be equal to the sum of the V multiplicities - V degree - 1, which must be larger than 1.");
				}
			}
		}
		catch (const StdFail_NotDone&) // from BRepBuilderAPI_MakeEdge
		{
			Throw(occtMakeFace);
		}

		for (const std::shared_ptr<Wire>& kInnerWire : rkInnerWires)
		{
			occtMakeFace.Add(kInnerWire->GetOcctWire());
		}

		std::shared_ptr<Face> pFace = std::make_shared<Face>(occtMakeFace);

		/*for (std::list<std::shared_ptr<Wire>>::const_iterator kInnerWireIterator = rkInnerWires.begin();
			kInnerWireIterator != rkInnerWires.end();
			kInnerWireIterator++)
		{
			const std::shared_ptr<Wire>& pkWire = *kInnerWireIterator;
			pkWire->AddIngredientTo(pFace);
		}*/

		/*if(kpOuterWire != nullptr)
		{
			kpOuterWire->AddIngredientTo(pFace);
		}*/
		return pFace;
	}

	void Face::SharedEdges(const std::shared_ptr<Face>& kpAnotherFace, std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared edges.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(GetOcctShape(), kpAnotherFace->GetOcctShape()).Shape();

		TopTools_MapOfShape occtEdges;
		for (TopExp_Explorer occtExplorer(rkOcctShape, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges.Contains(occtCurrent))
			{
				occtEdges.Add(occtCurrent);
			}
		}


		for (TopTools_MapIteratorOfMapOfShape kOcctMapIterator(occtEdges);
			kOcctMapIterator.More();
			kOcctMapIterator.Next())
		{
			rEdges.push_back(std::make_shared<Edge>(TopoDS::Edge(kOcctMapIterator.Value())));
		}
	}

	void Face::SharedVertices(const std::shared_ptr<Face>& kpAnotherFace, std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared vertices.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(GetOcctShape(), kpAnotherFace->GetOcctShape()).Shape();

		TopTools_MapOfShape occtVertices;
		for (TopExp_Explorer occtExplorer(GetOcctShape(), TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices.Contains(occtCurrent))
			{
				occtVertices.Add(occtCurrent);
			}
		}

		for (TopTools_MapIteratorOfMapOfShape kOcctMapIterator(occtVertices);
			kOcctMapIterator.More();
			kOcctMapIterator.Next())
		{
			rVertices.push_back(std::make_shared<Vertex>(TopoDS::Vertex(kOcctMapIterator.Value())));
		}
	}

	std::shared_ptr<Wire> Face::OuterBoundary() const
	{
		TopoDS_Wire occtOuterWire = ShapeAnalysis::OuterWire(GetOcctFace());
		return std::make_shared<Wire>(occtOuterWire);
	}

	void Face::InnerBoundaries(std::list<std::shared_ptr<Wire>>& rInnerBoundaries) const
	{
		const TopoDS_Face& rkFace = GetOcctFace();
		TopoDS_Wire occtOuterWire = ShapeAnalysis::OuterWire(rkFace);
		TopoDS_Iterator occtExplorer(rkFace, Standard_False);
		while (occtExplorer.More())
		{
			if (occtExplorer.Value().ShapeType() != TopAbs_WIRE)
				continue;
			const TopoDS_Wire& rkWire = TopoDS::Wire(occtExplorer.Value());

			if (!rkWire.IsSame(occtOuterWire))
			{
				rInnerBoundaries.push_back(std::make_shared<Wire>(rkWire));
			}
			occtExplorer.Next();
		}
	}

	void Face::UVParameterAtPoint(const std::shared_ptr<Vertex>& kpVertex, double& rU, double& rV) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctSurface);
		gp_Pnt2d occtUV = occtSurfaceAnalysis.ValueOfUV(kpVertex->Point()->Pnt(), Precision::Confusion());

		// Non-normalized UV
		double occtU = occtUV.X();
		double occtV = occtUV.Y();

		NormalizeUV(occtU, occtV, rU, rV);
	}

	std::shared_ptr<Vertex> Face::PointAtParameter(const double kU, const double kV) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		double occtU = 0.0, occtV = 0.0;
		NonNormalizeUV(kU, kV, occtU, occtV);

		ShapeAnalysis_Surface occtSurfaceAnalysis(Surface());
		gp_Pnt occtPoint = occtSurfaceAnalysis.Value(occtU, occtV);

		return Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));
	}

	std::shared_ptr<Face> Face::Trim(const std::shared_ptr<Wire>& kpWire) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		const TopoDS_Wire& rkWire = kpWire->GetOcctWire();

		BRepBuilderAPI_MakeFace occtTrimMakeFace(pOcctSurface, rkWire);
		if (occtTrimMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			Throw(occtTrimMakeFace);
		}
		return TopologicalQuery::Downcast<Face>(Topology::ByOcctShape(occtTrimMakeFace.Shape()));
	}

	void Face::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		rOcctGeometries.push_back(Surface());
	}

	TopoDS_Shape& Face::GetOcctShape()
	{
		return GetOcctFace();
	}

	const TopoDS_Shape& Face::GetOcctShape() const
	{
		return GetOcctFace();
	}

	TopoDS_Face& Face::GetOcctFace()
	{
		assert(m_occtFace.IsNull() && "Face::m_occtFace is null.");
		if (m_occtFace.IsNull())
		{
			throw std::exception("Face::m_occtFace is null.");
		}

		return m_occtFace;
	}

	const TopoDS_Face& Face::GetOcctFace() const
	{
		assert(m_occtFace.IsNull() && "Face::m_occtFace is null.");
		if (m_occtFace.IsNull())
		{
			throw std::exception("Face::m_occtFace is null.");
		}

		return m_occtFace;
	}

	Face::Face(const TopoDS_Face& rkOcctFace)
		: Topology(2)
		, m_occtFace(rkOcctFace)
	{
		try{
			/*ShapeFix_Face occtShapeFix(rkOcctFace);
			occtShapeFix.Perform();
			m_occtFace = std::make_shared<TopoDS_Face>(occtShapeFix.Face());*/

			GlobalCluster::GetInstance().Add(this);
		}
		catch (...)
		{
			throw std::exception("");
		}
	}

	Face::~Face()
	{
		GlobalCluster::GetInstance().Remove(this);
	}

	Handle(Geom_Surface) Face::Surface() const
	{
		TopoDS_Face occtFace = GetOcctFace();
		return BRep_Tool::Surface(occtFace);
	}

	void Face::Throw(const BRepBuilderAPI_MakeFace& rkOcctMakeFace)
	{
		// The error messages are based on those in the OCCT documentation.
		// https://www.opencascade.com/doc/occt-7.2.0/refman/html/_b_rep_builder_a_p_i___face_error_8hxx.html#ac7a498a52546f7535a3f73f6bab1599a

		switch (rkOcctMakeFace.Error())
		{
		case BRepBuilderAPI_NoFace:
			throw std::exception("No initialization of the algorithm; only an empty constructor was used.");

		case BRepBuilderAPI_NotPlanar:
			throw std::exception("No surface was given and the wire was not planar.");

		case BRepBuilderAPI_CurveProjectionFailed:
			throw std::exception("Curve projection failed.");

		case BRepBuilderAPI_ParametersOutOfRange:
			throw std::exception("The parameters given to limit the surface are out of its bounds.");

		default: // i.e. BRepBuilderAPI_FaceDone 
			throw std::exception("A face was successfully created.");
		}
	}

	void Face::NormalizeUV(const double kNonNormalizedU, const double kNonNormalizedV, double& rNormalizedU, double& rNormalizedV) const
	{
		double occtUMin = 0.0, occtUMax = 0.0, occtVMin = 0.0, occtVMax = 0.0;
		ShapeFix_Face occtShapeFix(GetOcctFace());
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

	void Face::NonNormalizeUV(const double kNormalizedU, const double kNormalizedV, double& rNonNormalizedU, double& rNonNormalizedV) const
	{
		double occtUMin = 0.0, occtUMax = 0.0, occtVMin = 0.0, occtVMax = 0.0;
		ShapeFix_Face occtShapeFix(GetOcctFace());
		occtShapeFix.Perform();
		ShapeAnalysis::GetFaceUVBounds(occtShapeFix.Face(), occtUMin, occtUMax, occtVMin, occtVMax);
		double occtDU = occtUMax - occtUMin;
		double occtDV = occtVMax - occtVMin;

		rNonNormalizedU = occtUMin + kNormalizedU * occtDU;
		rNonNormalizedV = occtVMin + kNormalizedV * occtDV;
	}
}