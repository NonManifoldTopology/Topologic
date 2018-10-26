#include "Face.h"
#include "Edge.h"
#include "Cell.h"
#include "Shell.h"
#include "Vertex.h"
#include "Wire.h"
#include "FaceFactory.h"
#include "Utilities.h"
#include "GlobalCluster.h"

#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BSplCLib.hxx>
#include <BRepGProp.hxx>
#include <Geom_BSplineSurface.hxx>
#include <GProp_GProps.hxx>
#include <ShapeAnalysis.hxx>
#include <ShapeFix_Face.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopOpeBRepBuild_Tools.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Face::AdjacentFaces(std::list<Face::Ptr>& rFaces) const
	{
		std::list<Vertex::Ptr> edges;
		Vertices(edges);

		TopTools_ListOfShape occtFaces;
		for (const Vertex::Ptr& kpEdge : edges)
		{
			std::list<Face::Ptr> faces;
			kpEdge->UpwardNavigation(faces);

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

	void Face::Cells(std::list<Cell::Ptr>& rCells) const
	{
		UpwardNavigation(rCells);
	}

	void Face::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Face::Shells(std::list<Shell::Ptr>& rShells) const
	{
		UpwardNavigation(rShells);
	}

	void Face::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Face::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
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
		std::list<Wire::Ptr> internalBoundaries;
		return ByExternalInternalBoundaries(kpExternalBoundary, internalBoundaries);
	}

	Face::Ptr Face::ByExternalInternalBoundaries(
		const Wire::Ptr& pkExternalBoundary,
		const std::list<Wire::Ptr>& rkInternalBoundaries)
	{
		Wire::Ptr copyExternalBoundary = std::dynamic_pointer_cast<Wire>(pkExternalBoundary->Copy());
		BRepBuilderAPI_MakeFace occtMakeFace(pkExternalBoundary->GetOcctWire());
		if (occtMakeFace.Error() != BRepBuilderAPI_FaceDone)
		{
			Throw(occtMakeFace);
		}

		for (const Wire::Ptr& kpInternalBoundary : rkInternalBoundaries)
		{
			Wire::Ptr pCopyInternalBoundary = std::dynamic_pointer_cast<Wire>(kpInternalBoundary->Copy());
			occtMakeFace.Add(pCopyInternalBoundary->GetOcctWire());
		}

		Face::Ptr pFace = std::make_shared<Face>(occtMakeFace);
		GlobalCluster::GetInstance().AddTopology(pFace->GetOcctFace());
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
		GlobalCluster::GetInstance().AddTopology(pFace->GetOcctFace());
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

			int occtNumberOfPoles = BSplCLib::NbPoles(kUDegree, kIsUPeriodic, rkOcctUMultiplicities);
			if (rkOcctPoles.Length() != occtNumberOfPoles)
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
		GlobalCluster::GetInstance().AddTopology(pFace->GetOcctFace());

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

	bool Face::IsManifold() const
	{
		std::list<Cell::Ptr> cells;
		Cells(cells);

		// A manifold face has 0 or 1 cell.
		if (cells.size() < 2)
		{
			return true;
		}

		return false;
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

	void Face::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		SetOcctFace(TopoDS::Face(rkOcctShape));
	}

	void Face::SetOcctFace(const TopoDS_Face & rkOcctFace)
	{
		m_occtFace = rkOcctFace;
	}

	Face::Face(const TopoDS_Face& rkOcctFace, const std::string& rkGuid)
		: Topology(2, rkOcctFace, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtFace(rkOcctFace)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<FaceFactory>());
	}

	Face::~Face()
	{

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
}