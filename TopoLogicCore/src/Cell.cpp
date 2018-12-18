#include "Cell.h"
#include "Vertex.h"
#include "Edge.h"
#include "Wire.h"
#include "Face.h"
#include "Shell.h"
#include "CellComplex.h"
#include "CellFactory.h"
#include "GlobalCluster.h"

#include <BRep_Builder.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepCheck_Shell.hxx>
#include <BRepClass3d.hxx>
#include <BRepGProp.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <GProp_GProps.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>

#include <TopTools_MapOfShape.hxx>
#include <BOPTools_AlgoTools.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Cell::AdjacentCells(std::list<Cell::Ptr>& rCells) const
	{
		// Iterate through the faces and find the incident cells which are not this cell.
		TopTools_IndexedDataMapOfShapeListOfShape occtFaceSolidMap;
		TopExp::MapShapesAndUniqueAncestors(GlobalCluster::GetInstance().GetOcctCompound(), TopAbs_FACE, TopAbs_SOLID, occtFaceSolidMap);

		// Find the constituent faces
		TopTools_MapOfShape occtFaces;
		DownwardNavigation(GetOcctShape(), TopAbs_FACE, occtFaces);

		const TopoDS_Solid& rkOcctSolid = GetOcctSolid();
		for (TopTools_MapOfShape::const_iterator kOcctFaceIterator = occtFaces.cbegin();
			kOcctFaceIterator != occtFaces.cend();
			kOcctFaceIterator++)
		{
			const TopoDS_Shape& rkOcctFace = *kOcctFaceIterator;
			const TopTools_ListOfShape& rkIncidentCells = occtFaceSolidMap.FindFromKey(rkOcctFace);

			for (TopTools_ListOfShape::const_iterator kOcctCellIterator = rkIncidentCells.cbegin();
				kOcctCellIterator != rkIncidentCells.cend();
				kOcctCellIterator++)
			{
				const TopoDS_Shape& rkIncidentCell = *kOcctCellIterator;
				if (!rkOcctSolid.IsSame(rkIncidentCell))
				{
					rCells.push_back(std::make_shared<Cell>(TopoDS::Solid(rkIncidentCell)));
				}
			}
		}
	}

	void Cell::CellComplexes(std::list<std::shared_ptr<TopologicCore::CellComplex>>& rCellComplexes) const
	{
		UpwardNavigation(rCellComplexes);
	}

	void Cell::Shells(std::list<Shell::Ptr>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void Cell::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Cell::Faces(std::list<Face::Ptr>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void Cell::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Cell::Wires(std::list<Wire::Ptr>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	Vertex::Ptr Cell::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	Cell::Ptr Cell::ByFaces(const std::list<Face::Ptr>& rkFaces, double kTolerance)
	{
		if (rkFaces.empty())
		{
			throw std::exception("No face is passed.");
		}

		Shell::Ptr pShell = Shell::ByFaces(rkFaces, kTolerance);
		Cell::Ptr pCell = ByShell(pShell);
		return pCell;
	}

	Cell::Ptr Cell::ByShell(const Shell::Ptr& kpShell)
	{
		BRepBuilderAPI_MakeSolid occtMakeSolid;
		try {
			Shell::Ptr pCopyShell = std::dynamic_pointer_cast<Shell>(kpShell);
			occtMakeSolid = BRepBuilderAPI_MakeSolid(pCopyShell->GetOcctShell());
		}
		catch (StdFail_NotDone&)
		{
			throw std::exception("The solid was not built.");
		}

		// Create a cell from the shell. The faces are the same and the contents
		// are automatically passed.
		Cell::Ptr pCell = std::make_shared<Cell>(occtMakeSolid);
		GlobalCluster::GetInstance().AddTopology(pCell->GetOcctSolid());
		return pCell;
	}

	void Cell::SharedEdges(const Cell::Ptr& kpAnotherCell, std::list<Edge::Ptr>& rEdges) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtEdges1;
		DownwardNavigation(rkOcctShape1, TopAbs_EDGE, occtEdges1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_MapOfShape occtEdges2;
		DownwardNavigation(rkOcctShape2, TopAbs_EDGE, occtEdges2);

		for (TopTools_MapIteratorOfMapOfShape occtEdgeIterator1(occtEdges1);
			occtEdgeIterator1.More();
			occtEdgeIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtEdgeIterator2(occtEdges2);
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

	void Cell::SharedFaces(const Cell::Ptr& kpAnotherCell, std::list<Face::Ptr>& rFaces) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtFaces1;
		DownwardNavigation(rkOcctShape1, TopAbs_FACE, occtFaces1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_MapOfShape occtFaces2;
		DownwardNavigation(rkOcctShape2, TopAbs_FACE, occtFaces2);

		for (TopTools_MapIteratorOfMapOfShape occtFaceIterator1(occtFaces1);
			occtFaceIterator1.More();
			occtFaceIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtFaceIterator2(occtFaces2);
				occtFaceIterator2.More();
				occtFaceIterator2.Next())
			{
				if (occtFaceIterator1.Value().IsSame(occtFaceIterator2.Value()))
				{
					Face::Ptr pFace = std::make_shared<Face>(TopoDS::Face(occtFaceIterator1.Value()));
					rFaces.push_back(pFace);
				}
			}
		}
	}

	void Cell::SharedVertices(const Cell::Ptr& kpAnotherCell, std::list<Vertex::Ptr>& rVertices) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtVertices1;
		DownwardNavigation(rkOcctShape1, TopAbs_VERTEX, occtVertices1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherCell->GetOcctShape();
		TopTools_MapOfShape occtVertices2;
		DownwardNavigation(rkOcctShape2, TopAbs_VERTEX, occtVertices2);

		for (TopTools_MapIteratorOfMapOfShape occtVertexIterator1(occtVertices1);
			occtVertexIterator1.More();
			occtVertexIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtVertexIterator2(occtVertices2);
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

	Shell::Ptr Cell::ExternalBoundary() const
	{
		TopoDS_Shell occtOuterShell = BRepClass3d::OuterShell(TopoDS::Solid(GetOcctShape()));
		return std::make_shared<Shell>(occtOuterShell);
	}

	void Cell::InternalBoundaries(std::list<Shell::Ptr>& rShells) const
	{
		Shell::Ptr pExternalBoundary = ExternalBoundary();

		std::list<Shell::Ptr> shells;
		DownwardNavigation(shells);
		for (const Shell::Ptr& kpShell : shells)
		{
			if (!kpShell->IsSame(pExternalBoundary))
			{
				rShells.push_back(kpShell);
			}
		}
	}

	bool Cell::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	TopoDS_Shape& Cell::GetOcctShape()
	{
		return GetOcctSolid();
	}

	const TopoDS_Shape& Cell::GetOcctShape() const
	{
		return GetOcctSolid();
	}

	TopoDS_Solid& Cell::GetOcctSolid()
	{
		assert(!m_occtSolid.IsNull() && "Cell::m_occtSolid is null.");
		if (m_occtSolid.IsNull())
		{
			throw std::exception("Cell::m_occtSolid is null.");
		}

		return m_occtSolid;
	}

	const TopoDS_Solid& Cell::GetOcctSolid() const
	{
		assert(!m_occtSolid.IsNull() && "Cell::m_occtSolid is null.");
		if (m_occtSolid.IsNull())
		{
			throw std::exception("Cell::m_occtSolid is null.");
		}

		return m_occtSolid;
	}

	void Cell::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		SetOcctSolid(TopoDS::Solid(rkOcctShape));
	}

	void Cell::SetOcctSolid(const TopoDS_Solid & rkOcctSolid)
	{
		m_occtSolid = rkOcctSolid;
	}

	void Cell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<Face::Ptr> faces;
		Faces(faces);
		for (const Face::Ptr& kpFace : faces)
		{
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	std::string Cell::GetTypeAsString() const
	{
		return std::string("Cell");
	}

	Cell::Cell(const TopoDS_Solid& rkOcctSolid, const std::string& rkGuid)
		: Topology(3, rkOcctSolid, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtSolid(rkOcctSolid)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<CellFactory>());
	}

	Cell::~Cell()
	{

	}
}
