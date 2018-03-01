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
#include <Geom_BSplineSurface.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeFix_Wire.hxx>
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
	void Face::AdjacentFaces(std::list<Face*>& rFaces) const
	{
		// Iterate through the edges and find the incident faces which are not this face.
		TopTools_IndexedDataMapOfShapeListOfShape occtEdgeFaceMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_EDGE, TopAbs_FACE, occtEdgeFaceMap);

		TopTools_MapOfShape occtWires;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtWires.Contains(occtCurrent))
			{
				occtWires.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtWires.cbegin();
			kOcctShapeIterator != occtWires.cend();
			kOcctShapeIterator++)
		{
			BRepTools_WireExplorer occtWireExplorer;
			for (occtWireExplorer.Init(TopoDS::Wire(*kOcctShapeIterator)); occtWireExplorer.More(); occtWireExplorer.Next())
			{
				TopoDS_Face occtFace;
				bool faceFound = TopOpeBRepBuild_Tools::GetAdjacentFace(*GetOcctShape(), occtWireExplorer.Current(), occtEdgeFaceMap, occtFace);
				if (faceFound)
				{
					rFaces.push_back(new Face(occtFace));
				}
			}
		}
	}

	void Face::Apertures(std::list<Face*>& rFaces) const
	{
		throw std::exception("Not implemented yet");
	}

	void Face::Cells(std::list<Cell*>& rCells) const
	{
		TopTools_IndexedDataMapOfShapeListOfShape occtFaceToCellssMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_FACE, TopAbs_SOLID, occtFaceToCellssMap);

		const TopTools_ListOfShape& rkOcctCells = occtFaceToCellssMap.FindFromKey(*GetOcctShape());

		for (TopTools_ListOfShape::const_iterator kIterator = rkOcctCells.cbegin();
			kIterator != rkOcctCells.cend();
			kIterator++)
		{
			if (kIterator->ShapeType() == TopAbs_SOLID)
			{
				const TopoDS_Solid& rkOcctSolid = TopoDS::Solid(*kIterator);
				rCells.push_back(new Cell(rkOcctSolid));
			}
		}
	}

	void Face::Edges(std::list<Edge*>& rEdges) const
	{
		TopTools_MapOfShape occtEdges;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges.Contains(occtCurrent))
			{
				occtEdges.Add(occtCurrent);
				rEdges.push_back(new Edge(TopoDS::Edge(occtCurrent)));
			}
		}
	}

	bool Face::IsApplied() const
	{
		throw std::exception("Not implemented yet");
		return false;
	}

	void Face::Shells(std::list<Shell*>& rShells) const
	{
		TopTools_IndexedDataMapOfShapeListOfShape occtFaceToShellsMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_FACE, TopAbs_SHELL, occtFaceToShellsMap);

		const TopTools_ListOfShape& rkOcctShells = occtFaceToShellsMap.FindFromKey(*GetOcctShape());

		for (TopTools_ListOfShape::const_iterator kIterator = rkOcctShells.cbegin();
			kIterator != rkOcctShells.cend();
			kIterator++)
		{
			if (kIterator->ShapeType() == TopAbs_SHELL)
			{
				const TopoDS_Shell& pOcctShell = TopoDS::Shell(*kIterator);
				rShells.push_back(new Shell(pOcctShell));
			}
		}
	}

	void Face::Vertices(std::list<Vertex*>& rVertices) const
	{
		TopTools_MapOfShape occtVertices;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices.Contains(occtCurrent))
			{
				occtVertices.Add(occtCurrent);
				rVertices.push_back(new Vertex(TopoDS::Vertex(occtCurrent)));
			}
		}
	}

	void Face::Wires(std::list<Wire*>& rWires) const
	{
		TopExp_Explorer occtExplorer;
		TopTools_MapOfShape occtWires;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_WIRE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtWires.Contains(occtCurrent))
			{
				occtWires.Add(occtCurrent);
				rWires.push_back(new Wire(TopoDS::Wire(occtCurrent)));
			}
		}
	}

	bool Face::ApplyApertures(const std::list<Face*>& rkApertures) const
	{
		throw std::exception("Not implemented yet");
		return false;
	}

	Face* Face::ByClosedWire(Wire const * const kpkWire)
	{
		BRepBuilderAPI_MakeFace occtMakeFace;
		try {
			occtMakeFace = BRepBuilderAPI_MakeFace(TopoDS::Wire(*kpkWire->GetOcctShape()));
			return new Face(occtMakeFace);
		}
		catch (StdFail_NotDone&)
		{
			throw new std::exception(GetOcctMakeFaceErrorMessage(occtMakeFace).c_str());
		}
	}

	Face* Face::ByEdges(const std::list<Edge*>& rkEdges)
	{
		return ByClosedWire(Wire::ByEdges(rkEdges));
	}

	Face* Face::BySurface(Handle(Geom_Surface) pOcctSurface)
	{
		BRepBuilderAPI_MakeFace occtMakeFace;
		try {
			occtMakeFace = BRepBuilderAPI_MakeFace(pOcctSurface, Precision::Confusion());
			return new Face(occtMakeFace);
		}
		catch (StdFail_NotDone&)
		{
			throw new std::exception(GetOcctMakeFaceErrorMessage(occtMakeFace).c_str());
		}
	}

	Face* Face::BySurface(
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
		Wire const * const kpkOuterWire,
		const std::list<Wire*>& rkInnerWires)
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
			if (kpkOuterWire != nullptr)
			{
				occtMakeFace = BRepBuilderAPI_MakeFace(pOcctBSplineSurface, TopoDS::Wire(kpkOuterWire->GetOcctShape()->Reversed()), true);
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
			throw std::exception(GetOcctMakeFaceErrorMessage(occtMakeFace).c_str());
		}

		for (std::list<Wire*>::const_iterator kInnerWireIterator = rkInnerWires.begin();
			kInnerWireIterator != rkInnerWires.end();
			kInnerWireIterator++)
		{
			occtMakeFace.Add(TopoDS::Wire(*(*kInnerWireIterator)->GetOcctShape()));
		}

		return new Face(occtMakeFace);
	}

	void Face::SharedEdges(Face const * const kpkAnotherFace, std::list<Edge*>& rEdges) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared edges.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(*GetOcctShape(), *kpkAnotherFace->GetOcctShape()).Shape();

		TopTools_MapOfShape occtEdges;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(rkOcctShape, TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges.Contains(occtCurrent))
			{
				occtEdges.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtEdges.cbegin();
			kOcctShapeIterator != occtEdges.cend();
			kOcctShapeIterator++)
		{
			rEdges.push_back(new Edge(TopoDS::Edge(*kOcctShapeIterator)));
		}
	}

	void Face::SharedVertices(Face const * const kpkAnotherFace, std::list<Vertex*>& rVertices) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared vertices.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(*GetOcctShape(), *kpkAnotherFace->GetOcctShape()).Shape();

		TopTools_MapOfShape occtVertices;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtVertices.Contains(occtCurrent))
			{
				occtVertices.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtVertices.cbegin();
			kOcctShapeIterator != occtVertices.cend();
			kOcctShapeIterator++)
		{
			rVertices.push_back(new Vertex(TopoDS::Vertex(*kOcctShapeIterator)));
		}
	}

	Wire* Face::OuterWire() const
	{
		return new Wire(ShapeAnalysis::OuterWire(TopoDS::Face(*GetOcctShape())));
	}

	void Face::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		rOcctGeometries.push_back(Surface());
	}

	TopoDS_Shape* Face::GetOcctShape() const
	{
		assert(m_pOcctFace != nullptr && "Face::m_pOcctFace is null.");
		if (m_pOcctFace == nullptr)
		{
			throw std::exception("Face::m_pOcctFace is null.");
		}

		return m_pOcctFace;
	}

	Face::Face(const TopoDS_Face& rkOcctFace)
		: Topology(2)
		, m_pOcctFace(nullptr)
	{
		ShapeFix_Face occtShapeFix(rkOcctFace);
		occtShapeFix.Perform();
		m_pOcctFace = new TopoDS_Face(occtShapeFix.Face());
		GlobalCluster::GetInstance().Add(this);
	}

	Face::~Face()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctFace;
	}

	Handle(Geom_Surface) Face::Surface() const
	{
		TopoDS_Face occtFace = TopoDS::Face(*GetOcctShape());
		return BRep_Tool::Surface(occtFace);
	}

	std::string Face::GetOcctMakeFaceErrorMessage(const BRepBuilderAPI_MakeFace& rkOcctMakeFace)
	{
		// The error messages are based on those in the OCCT documentation.
		// https://www.opencascade.com/doc/occt-7.2.0/refman/html/_b_rep_builder_a_p_i___face_error_8hxx.html#ac7a498a52546f7535a3f73f6bab1599a

		switch (rkOcctMakeFace.Error())
		{
		case BRepBuilderAPI_NoFace:
			return std::string("No initialization of the algorithm; only an empty constructor was used.");

		case BRepBuilderAPI_NotPlanar:
			return std::string("No surface was given and the wire was not planar.");

		case BRepBuilderAPI_CurveProjectionFailed:
			return std::string("Curve projection failed.");

		case BRepBuilderAPI_ParametersOutOfRange:
			return std::string("The parameters given to limit the surface are out of its bounds.");

		default: // i.e. BRepBuilderAPI_FaceDone 
			return std::string("A face was successfully created.");
		}
	}
}
