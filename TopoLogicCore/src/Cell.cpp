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
#include <BRepCheck_Shell.hxx>
#include <BRepClass3d.hxx>
#include <BRepGProp.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <GProp_GProps.hxx>
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
	void Cell::AdjacentCells(std::list<std::shared_ptr<Cell>>& rcells) const
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

		std::shared_ptr<TopoDS_Shape> pOcctSolid = GetOcctShape();
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
					rcells.push_back(std::make_shared<Cell>(TopoDS::Solid(rkIncidentCell)));
				}
			}
		}
	}

	void Cell::CellComplexes(std::list<std::shared_ptr<TopoLogicCore::CellComplex>>& rCellComplexes) const
	{
		UpwardNavigation(rCellComplexes);
	}

	void Cell::Shells(std::list<std::shared_ptr<Shell>>& rShells) const
	{
		DownwardNavigation(rShells);
	}

	void Cell::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		DownwardNavigation(rEdges);
	}

	void Cell::Faces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		DownwardNavigation(rFaces);
	}

	void Cell::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		DownwardNavigation(rVertices);
	}

	void Cell::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		DownwardNavigation(rWires);
	}

	double Cell::Volume() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(*GetOcctShape(), occtShapeProperties);
		return occtShapeProperties.Mass();
	}

	std::shared_ptr<Vertex> Cell::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::VolumeProperties(*GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	std::shared_ptr<Cell> Cell::ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces)
	{
		std::shared_ptr<Shell> pShell = Shell::ByFaces(rkFaces);
		std::shared_ptr<Cell> pCell = ByShell(pShell);
		for (std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = rkFaces.begin();
			kFaceIterator != rkFaces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			kpFace->AddIngredientTo(pCell);
		}
		return pCell;
	}

	std::shared_ptr<Cell> Cell::ByShell(const std::shared_ptr<Shell>& kpShells)
	{
		BRepBuilderAPI_MakeSolid occtMakeSolid;
		try {
			occtMakeSolid = BRepBuilderAPI_MakeSolid(TopoDS::Shell(*kpShells->GetOcctShape()));
		}
		catch (StdFail_NotDone&)
		{
			throw std::exception("The solid was not built.");
		}

		return std::make_shared<Cell>(occtMakeSolid);
	}

	std::shared_ptr<Cell> Cell::ByCuboid(Handle(Geom_CartesianPoint) pOcctCentroid, const double kXDimension, const double kYDimension, const double kZDimension)
	{
		gp_Pnt occtLowCorner(
			pOcctCentroid->X() - kXDimension / 2.0,
			pOcctCentroid->Y() - kYDimension / 2.0,
			pOcctCentroid->Z() - kZDimension / 2.0
			);
		BRepPrimAPI_MakeBox occtMakeBox(occtLowCorner, kXDimension, kYDimension, kZDimension);
		occtMakeBox.Build();

		return std::make_shared<Cell>(occtMakeBox.Solid());
	}

	std::shared_ptr<Cell> Cell::ByVerticesFaceIndices(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkFaceIndices)
	{
		std::list<std::shared_ptr<Face>> faces;
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
			faces.push_back(std::make_shared<Face>(occtMakeFace));
		}
		std::shared_ptr<Cell> pCell = ByFaces(faces);

		// Only add the vertices; the faces are dealt with in ByFaces()
		for (std::vector<std::shared_ptr<Vertex>>::const_iterator kVertexIterator = rkVertices.begin();
			kVertexIterator != rkVertices.end();
			kVertexIterator++)
		{
			const std::shared_ptr<Vertex>& kpVertex = *kVertexIterator;
			kpVertex->AddIngredientTo(pCell);
		}
		return pCell;
	}

	void Cell::SharedEdges(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared edges.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(*GetOcctShape(), *kpAnotherCell->GetOcctShape()).Shape();

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
			rEdges.push_back(std::make_shared<Edge>(TopoDS::Edge(*kOcctShapeIterator)));
		}
	}

	void Cell::SharedFaces(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Face>>& rFaces) const
	{
		// use BRepAlgoAPI_Common
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Common(*GetOcctShape(), *kpAnotherCell->GetOcctShape()).Shape();

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
			rFaces.push_back(std::make_shared<Face>(TopoDS::Face(*kOcctShapeIterator)));
		}
	}

	void Cell::SharedVertices(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		// BRepAlgoAPI_Section only returns vertices and edges, so use it to get the shared vertices.
		const TopoDS_Shape& rkOcctShape = BRepAlgoAPI_Section(*GetOcctShape(), *kpAnotherCell->GetOcctShape()).Shape();

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
			rVertices.push_back(std::make_shared<Vertex>(TopoDS::Vertex(*kOcctShapeIterator)));
		}
	}

	std::shared_ptr<Shell> Cell::OuterBoundary() const
	{
		TopoDS_Shell occtOuterShell = BRepClass3d::OuterShell(TopoDS::Solid(*GetOcctShape()));
		return std::make_shared<Shell>(occtOuterShell);
	}

	void Cell::InnerBoundaries(std::list<std::shared_ptr<Shell>>& rShells) const
	{
		TopTools_MapOfShape occtShells;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_SHELL); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtShells.Contains(occtCurrent))
			{
				TopAbs_Orientation occtOrientation;
				TopoDS_Iterator occtIterator;

				occtIterator.Initialize(occtCurrent);
				if (occtIterator.More()) {
					const TopoDS_Shape& rkMember = occtIterator.Value();
					occtOrientation = rkMember.Orientation();
					if (occtOrientation == TopAbs_INTERNAL)
					{
						// Only include an internal shell if it is closed
						const TopoDS_Shell& rkShell = TopoDS::Shell(occtCurrent);
						if (BRepCheck_Shell(rkShell).Closed())
						{
							occtShells.Add(occtCurrent);
							rShells.push_back(std::make_shared<Shell>(rkShell));
						}
					}
				}
			}
		}
	}

	std::shared_ptr<TopoDS_Shape> Cell::GetOcctShape() const
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
		std::list<std::shared_ptr<Face>> faces;
		Faces(faces);
		for (std::list<std::shared_ptr<Face>>::const_iterator kFaceIterator = faces.begin();
			kFaceIterator != faces.end();
			kFaceIterator++)
		{
			const std::shared_ptr<Face>& kpFace = *kFaceIterator;
			rOcctGeometries.push_back(kpFace->Surface());
		}
	}

	Cell::Cell(const TopoDS_Solid& rkOcctSolid)
		: Topology(3)
		, m_pOcctSolid(nullptr)
	{
		ShapeFix_Solid occtShapeFixSolid(rkOcctSolid);
		occtShapeFixSolid.Perform(Handle(Message_ProgressIndicator)());
		m_pOcctSolid = std::make_shared<TopoDS_Solid>(TopoDS::Solid(occtShapeFixSolid.Solid()));
		GlobalCluster::GetInstance().Add(this);
	}

	Cell::~Cell()
	{
		GlobalCluster::GetInstance().Remove(this);
	}
}
