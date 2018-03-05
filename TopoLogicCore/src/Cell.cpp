#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <GlobalCluster.h>
#include <CellComplex.h>

#include <BRep_Builder.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepClass3d.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <ShapeFix_Solid.hxx>
#include <StdFail_NotDone.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_FrozenShape.hxx>
#include <TopoDS_UnCompatibleShapes.hxx>

#include <TopTools_MapOfShape.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void Cell::AdjacentCells(std::list<Cell*>& rcells) const
	{
		// Iterate through the faces and find the incident cells which are not this cell.
		TopTools_IndexedDataMapOfShapeListOfShape occtFaceSolidMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_FACE, TopAbs_SOLID, occtFaceSolidMap);

		// Find the constituent faces
		TopTools_MapOfShape occtFaces;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(occtCurrent))
			{
				occtFaces.Add(occtCurrent);
			}
		}

		TopoDS_Shape* pOcctSolid = GetOcctShape();
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
				if (!pOcctSolid->IsSame(rkIncidentCell))
				{
					rcells.push_back(new Cell(TopoDS::Solid(rkIncidentCell)));
				}
			}
		}
	}

	CellComplex* Cell::CellComplex() const
	{
		TopTools_IndexedDataMapOfShapeListOfShape occtCellToCellComplexMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_SOLID, TopAbs_COMPSOLID, occtCellToCellComplexMap);

		const TopTools_ListOfShape& rkOcctCompSolids = occtCellToCellComplexMap.FindFromKey(*GetOcctShape());
		assert(rkOcctCompSolids.Size() < 2 && "There are more than 2 cell complexes containing this cell.");

		for (TopTools_ListOfShape::const_iterator kIterator = rkOcctCompSolids.cbegin();
			kIterator != rkOcctCompSolids.cend();
			kIterator++)
		{
			if (kIterator->ShapeType() == TopAbs_COMPSOLID)
			{
				return new TopoLogicCore::CellComplex(TopoDS::CompSolid(*kIterator));
			}
		}

		return nullptr;
	}

	void Cell::Shells(std::list<Shell*>& rShells) const
	{
		TopTools_MapOfShape occtShells;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_SHELL); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtShells.Contains(occtCurrent))
			{
				occtShells.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtShells.cbegin();
			kOcctShapeIterator != occtShells.cend();
			kOcctShapeIterator++)
		{
			rShells.push_back(new Shell(TopoDS::Shell(*kOcctShapeIterator)));
		}
	}

	void Cell::Edges(std::list<Edge*>& rEdges) const
	{
		TopTools_MapOfShape occtEdges;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
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

	void Cell::Faces(std::list<Face*>& rFaces) const
	{
		TopTools_MapOfShape occtFaces;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(occtCurrent))
			{
				occtFaces.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtFaces.cbegin();
			kOcctShapeIterator != occtFaces.cend();
			kOcctShapeIterator++)
		{
			rFaces.push_back(new Face(TopoDS::Face(*kOcctShapeIterator)));
		}
	}

	void Cell::Vertices(std::list<Vertex*>& rVertices) const
	{
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

	void Cell::Wires(std::list<Wire*>& rWires) const
	{
		TopTools_MapOfShape occtWires;
		TopExp_Explorer occtExplorer;
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

	Cell* Cell::ByFaces(const std::list<Face*>& rkFaces)
	{
		TopoDS_Shell occtShell;
		BRep_Builder occtBuilder;
		occtBuilder.MakeShell(occtShell);
		for(std::list<Face*>::const_iterator kFaceIterator = rkFaces.begin();
			kFaceIterator != rkFaces.end();
			kFaceIterator++)
		{
			Face* pFace = *kFaceIterator;
			try {
				occtBuilder.Add(occtShell, *pFace->GetOcctShape());
			}
			catch (TopoDS_FrozenShape&)
			{
				throw std::exception("The cell is not free and cannot be modified.");
			}
			catch (TopoDS_UnCompatibleShapes&)
			{
				throw std::exception("The cell and face are not compatible.");
			}
		}

		return ByShell(new Shell(occtShell));
	}

	Cell* Cell::ByShell(Shell const * const kpkShells)
	{
		BRepBuilderAPI_MakeSolid occtMakeSolid;
		try {
			occtMakeSolid = BRepBuilderAPI_MakeSolid(TopoDS::Shell(*kpkShells->GetOcctShape()));
		}
		catch (StdFail_NotDone&)
		{
			throw std::exception("The solid was not built.");
		}

		return new Cell(occtMakeSolid);
	}

	Cell* Cell::ByCuboid(Handle(Geom_CartesianPoint) pOcctCentroid, const double kXDimension, const double kYDimension, const double kZDimension)
	{
		gp_Pnt occtLowCorner(
			pOcctCentroid->X() - kXDimension / 2.0,
			pOcctCentroid->Y() - kYDimension / 2.0,
			pOcctCentroid->Z() - kZDimension / 2.0
			);
		BRepPrimAPI_MakeBox occtMakeBox(occtLowCorner, kXDimension, kYDimension, kZDimension);
		occtMakeBox.Build();

		return new Cell(occtMakeBox.Solid());
	}

	Cell* Cell::ByVerticesFaceIndices(const std::vector<Vertex*>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		std::list<Face*> faces;
		for (std::list<std::list<int>>::const_iterator kFaceIndexIterator = rkFaceIndices.begin();
			kFaceIndexIterator != rkFaceIndices.end();
			kFaceIndexIterator++)
		{
			const std::list<int>& rkVertexIndices = *kFaceIndexIterator;

			BRepBuilderAPI_MakeWire occtMakeWire;

			std::list<int>::const_iterator kSecondFromLastVertexIterator = --rkVertexIndices.end();
			for (std::list<int>::const_iterator kVertexIterator = rkVertexIndices.begin();
				kVertexIterator != kSecondFromLastVertexIterator;
				kVertexIterator++)
			{
				int vertexIndex = *kVertexIterator;

				std::list<int>::const_iterator kNextVertexIterator = kVertexIterator;
				kNextVertexIterator++;
				int nextVertexIndex = *kNextVertexIterator;

				occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
					TopoDS::Vertex(*rkVertices[vertexIndex]->GetOcctShape()),
					TopoDS::Vertex(*rkVertices[nextVertexIndex]->GetOcctShape())
				));
			}
			occtMakeWire.Add(BRepBuilderAPI_MakeEdge(
				TopoDS::Vertex(*rkVertices[*--rkVertexIndices.end()]->GetOcctShape()),
				TopoDS::Vertex(*rkVertices[*rkVertexIndices.begin()]->GetOcctShape())
			));

			TopoDS_Wire* pOcctWire = new TopoDS_Wire(occtMakeWire);
			BRepBuilderAPI_MakeFace occtMakeFace(*pOcctWire);
			faces.push_back(new Face(occtMakeFace));
		}
		return ByFaces(faces);
	}

	void Cell::SharedEdges(Cell const * const kpkAnotherCell, std::list<Edge*>& rEdges) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared edges.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(*GetOcctShape(), *kpkAnotherCell->GetOcctShape()).Shape();

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

	void Cell::SharedFaces(Cell const * const kpkAnotherCell, std::list<Face*>& rFaces) const
	{
		// use BRepAlgoAPI_Common
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Common(*GetOcctShape(), *kpkAnotherCell->GetOcctShape()).Shape();

		TopTools_MapOfShape occtFaces;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(rkOcctShape, TopAbs_FACE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtFaces.Contains(occtCurrent))
			{
				occtFaces.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtFaces.cbegin();
			kOcctShapeIterator != occtFaces.cend();
			kOcctShapeIterator++)
		{
			rFaces.push_back(new Face(TopoDS::Face(*kOcctShapeIterator)));
		}
	}

	void Cell::SharedVertices(Cell const * const kpkAnotherCell, std::list<Vertex*>& rVertices) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared vertices.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(*GetOcctShape(), *kpkAnotherCell->GetOcctShape()).Shape();

		TopTools_MapOfShape occtVertices;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(rkOcctShape, TopAbs_VERTEX); occtExplorer.More(); occtExplorer.Next())
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

	TopoDS_Shape* Cell::GetOcctShape() const
	{
		assert(m_pOcctSolid != nullptr && "Cell::m_pOcctSolid is null.");
		if (m_pOcctSolid == nullptr)
		{
			throw std::exception("Cell::m_pOcctSolid is null.");
		}

		return m_pOcctSolid;
	}

	void Cell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of faces
		std::list<Face*> faces;
		Faces(faces);
		for (std::list<Face*>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			Face* pFace = *kFaceIterator;
			rOcctGeometries.push_back(pFace->Surface());
		}
	}

	void Cell::InnerShells(std::list<Shell*>& rShells) const
	{
		TopTools_MapOfShape occtShells;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_SHELL); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtShells.Contains(occtCurrent))
			{
				bool isInternal = false;
				TopAbs_Orientation occtOrientation;
				TopoDS_Iterator occtIterator;
				
				occtIterator.Initialize(occtCurrent);
				if (occtIterator.More()) {
					const TopoDS_Shape& rkMember = occtIterator.Value();
					occtOrientation = rkMember.Orientation();
					isInternal = (occtOrientation == TopAbs_INTERNAL);
				}

				if(isInternal)
				{
					occtShells.Add(occtCurrent);
				}
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtShells.cbegin();
			kOcctShapeIterator != occtShells.cend();
			kOcctShapeIterator++)
		{
			rShells.push_back(new Shell(TopoDS::Shell(*kOcctShapeIterator)));
		}
	}

	void Cell::InnerShells(std::list<Shell*>& rShells) const
	{
		TopTools_MapOfShape occtShells;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_SHELL); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtShells.Contains(occtCurrent))
			{
				bool isInternal = false;
				TopAbs_Orientation occtOrientation;
				TopoDS_Iterator occtIterator;

				occtIterator.Initialize(occtCurrent);
				if (occtIterator.More()) {
					const TopoDS_Shape& rkMember = occtIterator.Value();
					occtOrientation = rkMember.Orientation();
					isInternal = (occtOrientation == TopAbs_INTERNAL);
				}

				if (isInternal)
				{
					occtShells.Add(occtCurrent);
				}
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtShells.cbegin();
			kOcctShapeIterator != occtShells.cend();
			kOcctShapeIterator++)
		{
			rShells.push_back(new Shell(TopoDS::Shell(*kOcctShapeIterator)));
		}
	}

	Cell::Cell(const TopoDS_Solid& rkOcctSolid)
		: Topology(3)
		, m_pOcctSolid(nullptr)
	{
		ShapeFix_Solid occtShapeFixSolid(rkOcctSolid);
		occtShapeFixSolid.Perform(Handle(Message_ProgressIndicator)());
		m_pOcctSolid = new TopoDS_Solid(TopoDS::Solid(occtShapeFixSolid.Solid()));
		GlobalCluster::GetInstance().Add(this);
	}

	Cell::~Cell()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctSolid;
	}
}
