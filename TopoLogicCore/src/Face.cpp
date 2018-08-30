#include <Face.h>
#include <Edge.h>
#include <Cell.h>
#include <Shell.h>
#include <Vertex.h>
#include <Wire.h>
#include <FaceFactory.h>
#include <Utilities.h>

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BSplCLib.hxx>
#include <BRepGProp.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <GeomConvert.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <ShapeFix_Face.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopOpeBRepBuild_Tools.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Face::AdjacentFaces(const Topology::Ptr& kpParentTopology, std::list<Face::Ptr>& rFaces) const
	{
		std::list<Vertex::Ptr> edges;
		Vertices(edges);

		TopTools_ListOfShape occtFaces;
		for (const Vertex::Ptr& kpEdge : edges)
		{
			std::list<Face::Ptr> faces;
			kpEdge->UpwardNavigation(kpParentTopology, faces);

			for (const Face::Ptr& kpFace : faces)
			{
				if (!IsSame(kpFace) && 
					!occtFaces.Contains(kpFace->GetOcctShape()))
				{
					occtFaces.Append(kpFace->GetOcctShape());
					rFaces.push_back(kpFace);
				}
			}
		}
	}

	void Face::Cells(const Topology::Ptr& kpParentTopology, std::list<Cell::Ptr>& rCells) const
	{
		Cells(kpParentTopology.get(), rCells);
	}

	void Face::Cells(Topology const * kpkParentTopology, std::list<std::shared_ptr<Cell>>& rCells) const
	{
		UpwardNavigation(kpkParentTopology, rCells);
	}

	void Face::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Face::Shells(const Topology::Ptr& kpParentTopology, std::list<Shell::Ptr>& rShells) const
	{
		UpwardNavigation(kpParentTopology, rShells);
	}

	void Face::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Face::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	double Face::Area() const
	{
		GProp_GProps occtShapeProperties;
		ShapeFix_Face occtShapeFix(GetOcctFace());
		occtShapeFix.Perform();
		BRepGProp::SurfaceProperties(occtShapeFix.Face(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

	Vertex::Ptr Face::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		ShapeFix_Face occtShapeFix(GetOcctFace());
		occtShapeFix.Perform();
		BRepGProp::SurfaceProperties(occtShapeFix.Face(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	Face::Ptr Face::ByExternalBoundary(const Wire::Ptr& kpExternalBoundary)
	{
		/*BRepBuilderAPI_MakeFace occtMakeFace(pkWire->GetOcctWire());
		if (occtMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			Throw(occtMakeFace);
		}

		Face::Ptr pFace = std::make_shared<Face>(occtMakeFace);*/
		
		// TODO: Add modified mebrs
		/*std::list<std::pair<Topology::Ptr, Topology::Ptr>> topologyPairs;
		std::list<Topology::Ptr> members;
		pFace->Members(members);

		for (const Topology::Ptr& kpMember : members)
		{
			topologyPairs.push_back(std::make_pair(kpMember, kpMember));
		}

		LabelManager::GetInstance().AddModifiedMembers(
			pFace->GetOcctLabel(),
			topologyPairs);*/

		std::list<Wire::Ptr> internalBoundaries;
		return ByExternalInternalBoundaries(kpExternalBoundary, internalBoundaries);
	}

	Face::Ptr Face::ByExternalInternalBoundaries(
		const Wire::Ptr& pkExternalBoundary,
		const std::list<Wire::Ptr>& rkInternalBoundaries)
	{
		BRepBuilderAPI_MakeFace occtMakeFace(pkExternalBoundary->GetOcctWire());
		if (occtMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			Throw(occtMakeFace);
		}

		for (const std::shared_ptr<Wire>& kpInternalBoundary : rkInternalBoundaries)
		{
			occtMakeFace.Add(kpInternalBoundary->GetOcctWire());
		}

		Face::Ptr pFace = std::make_shared<Face>(occtMakeFace);
		return pFace;
	}

	Face::Ptr Face::ByEdges(const std::list<Edge::Ptr>& rkEdges)
	{
		if (rkEdges.size())
		{
			throw std::exception("No edge is passed.");
		}
		
		Wire::Ptr pWire = Wire::ByEdges(rkEdges);
		Face::Ptr pFace = ByExternalBoundary(pWire);

		return pFace;
	}

	Face::Ptr Face::BySurface(Handle(Geom_Surface) pOcctSurface)
	{
		BRepBuilderAPI_MakeFace occtMakeFace;
		try {
			occtMakeFace = BRepBuilderAPI_MakeFace(pOcctSurface, Precision::Confusion());
		}
		catch (StdFail_NotDone&)
		{
			Throw(occtMakeFace);
		}
		Face::Ptr pFace = std::make_shared<Face>(occtMakeFace);
		//LabelManager::GetInstance().AddGeneratedMembersToLabel(pFace->GetOcctLabel());
		return pFace;
	}

	Face::Ptr Face::BySurface(
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
		const Wire::Ptr& kpOuterWire,
		const std::list<Wire::Ptr>& rkInnerWires)
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

		for (const Wire::Ptr& kInnerWire : rkInnerWires)
		{
			occtMakeFace.Add(kInnerWire->GetOcctWire());
		}

		Face::Ptr pFace = std::make_shared<Face>(occtMakeFace);
		std::list<std::pair<Topology::Ptr, Topology::Ptr>> topologyPairs;
		std::list<Topology::Ptr> members;
		pFace->Members(members);

		/*LabelManager::GetInstance().AddModifiedMembers(
			pFace->GetOcctLabel(),
			topologyPairs);*/
		return pFace;
	}

	void Face::SharedEdges(const Face::Ptr& kpAnotherFace, std::list<Edge::Ptr>& rEdges) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_ListOfShape occtEdges1;
		for (TopExp_Explorer occtExplorer(rkOcctShape1, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges1.Contains(occtCurrent))
			{
				occtEdges1.Append(occtCurrent);
			}
		}

		const TopoDS_Shape& rkOcctShape2 = kpAnotherFace->GetOcctShape();
		TopTools_ListOfShape occtEdges2;
		for (TopExp_Explorer occtExplorer(rkOcctShape2, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges2.Contains(occtCurrent))
			{
				occtEdges2.Append(occtCurrent);
			}
		}

		for (TopTools_ListIteratorOfListOfShape occtEdgeIterator1(occtEdges1);
			occtEdgeIterator1.More();
			occtEdgeIterator1.Next())
		{
			for (TopTools_ListIteratorOfListOfShape occtEdgeIterator2(occtEdges2);
				occtEdgeIterator2.More();
				occtEdgeIterator2.Next())
			{
				if (occtEdgeIterator1.Value().IsSame(occtEdgeIterator2.Value()))
				{
					Edge::Ptr pEdge = std::make_shared<Edge>(TopoDS::Edge(occtEdgeIterator1.Value()));
					rEdges.push_back(pEdge);
				}
			}
		}
	}

	void Face::SharedVertices(const Face::Ptr& kpAnotherFace, std::list<Vertex::Ptr>& rVertices) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_ListOfShape occtVertices1;
		for (TopExp_Explorer occtExplorer(rkOcctShape1, TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices1.Contains(occtCurrent))
			{
				occtVertices1.Append(occtCurrent);
			}
		}

		const TopoDS_Shape& rkOcctShape2 = kpAnotherFace->GetOcctShape();
		TopTools_ListOfShape occtVertices2;
		for (TopExp_Explorer occtExplorer(rkOcctShape2, TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices2.Contains(occtCurrent))
			{
				occtVertices2.Append(occtCurrent);
			}
		}

		for (TopTools_ListIteratorOfListOfShape occtVertexIterator1(occtVertices1);
			occtVertexIterator1.More();
			occtVertexIterator1.Next())
		{
			for (TopTools_ListIteratorOfListOfShape occtVertexIterator2(occtVertices2);
				occtVertexIterator2.More();
				occtVertexIterator2.Next())
			{
				if (occtVertexIterator1.Value().IsSame(occtVertexIterator2.Value()))
				{
					Vertex::Ptr pVertex = std::make_shared<Vertex>(TopoDS::Vertex(occtVertexIterator1.Value()));
					rVertices.push_back(pVertex);
				}
			}
		}
	}

	Face::Ptr Face::ByVertices(const std::list<std::list<TopologicCore::Vertex::Ptr>>& rkVertices)
	{
		// Check if all vertex lists have the same length
		int rowLength = 0;
		bool firstRow = true;
		for(const std::list<TopologicCore::Vertex::Ptr>& rkVerticesRow : rkVertices)
		{ 
			if (firstRow)
			{
				rowLength = (int) rkVerticesRow.size();
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
		int columnLength = (int) rkVertices.size();

		TColgp_Array2OfPnt occtPoints(1, columnLength, 1, rowLength);
		int i = 1;
		for (const std::list<TopologicCore::Vertex::Ptr>& rkVerticesRow : rkVertices)
		{
			int j = 1;
			for (const TopologicCore::Vertex::Ptr& kpVertex: rkVerticesRow)
			{
				occtPoints.SetValue(i, j, kpVertex->Point()->Pnt());
				j++;
			}
			i++;
		}
		try {
			Handle(Geom_Surface) pSurface = GeomAPI_PointsToBSplineSurface(occtPoints).Surface();
			return Face::BySurface(pSurface);
		}
		catch (...)
		{
			throw std::exception("Fails to create a face by vertices");
		}
	}

	Wire::Ptr Face::OuterBoundary() const
	{
		TopoDS_Wire occtOuterWire = ShapeAnalysis::OuterWire(GetOcctFace());

		return std::make_shared<Wire>(occtOuterWire);
	}

	void Face::InnerBoundaries(std::list<Wire::Ptr>& rInnerBoundaries) const
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

	void Face::AddInternalBoundary(const std::shared_ptr<Wire>& kpWire)
	{
		std::list<Wire::Ptr> wires;
		wires.push_back(kpWire);
		AddInternalBoundaries(wires);
	}

	void Face::AddInternalBoundaries(const std::list<std::shared_ptr<Wire>>& rkWires)
	{
		BRepBuilderAPI_MakeFace occtMakeFace(GetOcctFace());

		for (const Wire::Ptr& kpWire : rkWires)
		{
			occtMakeFace.Add(kpWire->GetOcctWire());
		}

		m_occtFace = occtMakeFace;
	}

	bool Face::IsManifold(TopologicCore::Topology const * const kpkParentTopology) const
	{
		std::list<Cell::Ptr> cells;
		Cells(kpkParentTopology, cells);

		// A manifold face has 0 or 1 cell.
		if (cells.size() < 2)
		{
			return true;
		}

		return false;
	}

	void Face::UVParameterAtPoint(const Vertex::Ptr& kpVertex, double& rU, double& rV) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		ShapeAnalysis_Surface occtSurfaceAnalysis(pOcctSurface);
		gp_Pnt2d occtUV = occtSurfaceAnalysis.ValueOfUV(kpVertex->Point()->Pnt(), Precision::Confusion());

		// Non-normalized UV
		double occtU = occtUV.X();
		double occtV = occtUV.Y();

		NormalizeUV(occtU, occtV, rU, rV);
	}

	Vertex::Ptr Face::PointAtParameter(const double kU, const double kV) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		double occtU = 0.0, occtV = 0.0;
		NonNormalizeUV(kU, kV, occtU, occtV);

		double occtMinU = 0.0, occtMaxU = 0.0, occtMinV = 0.0, occtMaxV = 0.0;
		ShapeAnalysis::GetFaceUVBounds(GetOcctFace(), occtMinU, occtMaxU, occtMinV, occtMaxV);
		ShapeAnalysis_Surface occtSurfaceAnalysis(Surface());
		const Handle(Geom_Surface)& rSurface = occtSurfaceAnalysis.Surface();
		Handle(Geom_RectangularTrimmedSurface) pTrimmedSurface = new Geom_RectangularTrimmedSurface(rSurface, occtMinU, occtMaxU, occtMinV, occtMaxV);
		Handle(Geom_BSplineSurface) pBSplineSurface = GeomConvert::SurfaceToBSplineSurface(pTrimmedSurface);
		//gp_Pnt occtPoint = occtSurfaceAnalysis.Value(occtU, occtV);
		gp_Pnt occtPoint = pBSplineSurface->Value(occtU, occtV);
		Vertex::Ptr vertex = Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));

		// Check distance: if more than a small aount, it is not part of the face.
		// Hack: project to surface
		double distance = Distance(vertex);
		if (distance > Precision::Confusion())
		{
			throw std::exception("Point not on the face.");
		}

		return Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));
	}

	gp_Dir Face::NormalAtParameter(const double kU, const double kV) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		double occtU = 0.0, occtV = 0.0;
		NonNormalizeUV(kU, kV, occtU, occtV);

		GeomLProp_SLProps occtProperties(Surface(), occtU, occtV, 1, Precision::Confusion());

		return occtProperties.Normal();
	}

	Face::Ptr Face::Trim(const Wire::Ptr& kpWire) const
	{
		Handle(Geom_Surface) pOcctSurface = Surface();
		const TopoDS_Wire& rkWire = kpWire->GetOcctWire();

		BRepBuilderAPI_MakeFace occtTrimMakeFace(pOcctSurface, rkWire);
		if (occtTrimMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			Throw(occtTrimMakeFace);
		}
		return std::make_shared<Face>(TopoDS::Face(occtTrimMakeFace.Shape()));
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
		assert(!m_occtFace.IsNull() && "Face::m_occtFace is null.");
		if (m_occtFace.IsNull())
		{
			throw std::exception("Face::m_occtFace is null.");
		}

		return m_occtFace;
	}

	const TopoDS_Face& Face::GetOcctFace() const
	{
		assert(!m_occtFace.IsNull() && "Face::m_occtFace is null.");
		if (m_occtFace.IsNull())
		{
			throw std::exception("Face::m_occtFace is null.");
		}

		return m_occtFace;
	}

	Face::Face(const TopoDS_Face& rkOcctFace, const std::string& rkGuid)
		: Topology(2, rkOcctFace, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtFace(rkOcctFace)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<FaceFactory>());
	}

	Face::~Face()
	{
		//GlobalCluster::GetInstance().Remove(this);
		//DecreaseCounter();
	}

	Handle(Geom_Surface) Face::Surface() const
	{
		TopoDS_Face occtFace = GetOcctFace();
		return BRep_Tool::Surface(occtFace);
	}

	std::string Face::GetTypeAsString() const
	{
		return std::string("Face");
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