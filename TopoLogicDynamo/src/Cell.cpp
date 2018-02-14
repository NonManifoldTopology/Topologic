#include <Cell.h>
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <CellComplex.h>

#include <Geom_CartesianPoint.hxx>
#include <Precision.hxx>

#include <assert.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Cell::ByFaces(List<Face^>^ faces)
	{
		Cell^ pCell = gcnew Cell(faces);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cell", pCell);
		pDictionary->Add("Solid", pCell->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::BySolid(Autodesk::DesignScript::Geometry::Solid^ solid)
	{
		Cell^ pCell = gcnew Cell(solid);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cell", pCell);
		pDictionary->Add("Solid", pCell->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::ByShell(Shell^ shell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Cell::ByShell(TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(shell->GetCoreTopology()));
		Cell^ pCell = gcnew Cell(pCoreCell);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cell", pCell);
		pDictionary->Add("Solid", pCell->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices)
	{
		std::vector<TopoLogicCore::Vertex*> coreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(pVertex->GetCoreTopology()));
		}

		std::list<std::list<int>> coreFaceIndices;
		for each(List<int>^ pFaceIndex in faceIndices)
		{
			std::list<int> coreFaceIndex;
			for each(int vertexIndex in pFaceIndex)
			{
				coreFaceIndex.push_back(vertexIndex);
			}
			coreFaceIndices.push_back(coreFaceIndex);
		}

		Cell^ pCell = gcnew Cell(TopoLogicCore::Cell::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cell", pCell);
		pDictionary->Add("Solid", pCell->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::CellComplex(Cell ^ topoLogicCell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(topoLogicCell->GetCoreTopology());

		TopoLogicCore::CellComplex* pCoreCellComplex = pCoreCell->CellComplex();
		TopoLogic::CellComplex^ pCellComplex = nullptr;
		List<System::Object^>^ pDynamoSolids = gcnew List<System::Object^>();
		
		if(pCoreCellComplex != nullptr)
		{
			pCellComplex = gcnew TopoLogic::CellComplex(pCoreCellComplex);
		
			std::list<TopoLogicCore::Cell*> coreCells;
			pCoreCellComplex->Cells(coreCells);

			for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = coreCells.begin();
				kCellIterator != coreCells.end();
				kCellIterator++)
			{
				Cell^ pCell = gcnew Cell(*kCellIterator);
				pDynamoSolids->Add(pCell->Geometry);
			}
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic CellComplex", pCellComplex);
		pDictionary->Add("Solids", pDynamoSolids);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::Faces(Cell^ topoLogicCell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(topoLogicCell->GetCoreTopology());

		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreCell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kFaceIterator = coreFaces.begin();
			kFaceIterator != coreFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pFaces->Add(pFace);
			pDynamoEntities->Add(pFace->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pFaces);
		pDictionary->Add("Surfaces", pDynamoEntities);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::Wires(Cell ^ topoLogicCell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(topoLogicCell->GetCoreTopology());

		std::list<TopoLogicCore::Wire*> coreWires;
		pCoreCell->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		List<System::Object^>^ pDynamoPolycurves = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Wire*>::const_iterator kWireIterator = coreWires.begin();
			kWireIterator != coreWires.end();
			kWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kWireIterator);
			pWires->Add(pWire);
			pDynamoPolycurves->Add(pWire->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Wires", pWires);
		pDictionary->Add("PolyCurves", pDynamoPolycurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::Edges(Cell^ topoLogicCell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(topoLogicCell->GetCoreTopology());

		std::list<TopoLogicCore::Edge*> coreEdges;
		pCoreCell->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		List<System::Object^>^ pDynamoCurves = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = coreEdges.begin();
			kEdgeIterator != coreEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pEdges->Add(pEdge);
			pDynamoCurves->Add(pEdge->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edges", pEdges);
		pDictionary->Add("Curves", pDynamoCurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::Vertices(Cell^ topoLogicCell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(topoLogicCell->GetCoreTopology());

		std::list<TopoLogicCore::Vertex*> coreVertices;
		pCoreCell->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		List<System::Object^>^ pDynamoPoints = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = coreVertices.begin();
			kVertexIterator != coreVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pVertices->Add(pVertex);
			pDynamoPoints->Add(pVertex->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertices", pVertices);
		pDictionary->Add("Points", pDynamoPoints);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::AdjacentCells(Cell^ cell)
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell->GetCoreTopology());

		std::list<TopoLogicCore::Cell*> coreAdjacentCells;
		pCoreCell->AdjacentCells(coreAdjacentCells);

		List<Cell^>^ pAdjacentCells = gcnew List<Cell^>();
		List<System::Object^>^ pDynamoSolids = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = coreAdjacentCells.begin();
			kCellIterator != coreAdjacentCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pAdjacentCells->Add(pCell);
			pDynamoSolids->Add(pCell->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cells", pAdjacentCells);
		pDictionary->Add("Solids", pDynamoSolids);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::SharedFaces(Cell^ cell1, Cell^ cell2)
	{
		TopoLogicCore::Cell* pCoreCell1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell1->GetCoreTopology());
		TopoLogicCore::Cell* pCoreCell2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell2->GetCoreTopology());

		std::list<TopoLogicCore::Face*> sharedFaces;
		pCoreCell1->SharedFaces(pCoreCell2, sharedFaces);

		List<Face^>^ pSharedFaces = gcnew List<Face^>();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for(std::list<TopoLogicCore::Face*>::const_iterator kFaceIterator = sharedFaces.begin();
			kFaceIterator != sharedFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pSharedFaces->Add(pFace);
			pDynamoEntities->Add(pFace->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pSharedFaces);
		pDictionary->Add("Surfaces", pDynamoEntities);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::SharedEdges(Cell^ cell1, Cell^ cell2)
	{
		TopoLogicCore::Cell* pCoreCell1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell1->GetCoreTopology());
		TopoLogicCore::Cell* pCoreCell2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell2->GetCoreTopology());

		std::list<TopoLogicCore::Edge*> sharedEdges;
		pCoreCell1->SharedEdges(pCoreCell2, sharedEdges);

		List<Edge^>^ pSharedEdges = gcnew List<Edge^>();
		List<System::Object^>^ pDynamoCurves = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = sharedEdges.begin();
			kEdgeIterator != sharedEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pSharedEdges->Add(pEdge);
			pDynamoCurves->Add(pEdge->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edges", pSharedEdges);
		pDictionary->Add("Curves", pDynamoCurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cell::SharedVertices(Cell^ cell1, Cell^ cell2)
	{
		TopoLogicCore::Cell* pCoreCell1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell1->GetCoreTopology());
		TopoLogicCore::Cell* pCoreCell2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(cell2->GetCoreTopology());

		std::list<TopoLogicCore::Vertex*> sharedVertices;
		pCoreCell1->SharedVertices(pCoreCell2, sharedVertices);

		List<Vertex^>^ pSharedVertices = gcnew List<Vertex^>();
		List<System::Object^>^ pDynamoPoints = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = sharedVertices.begin();
			kVertexIterator != sharedVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pSharedVertices->Add(pVertex);
			pDynamoPoints->Add(pVertex->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertices", pSharedVertices);
		pDictionary->Add("Points", pDynamoPoints);
		return pDictionary;
	}

	Object^ Cell::Geometry::get()
	{
		List<Autodesk::DesignScript::Geometry::Surface^>^ pDynamoSurfaces = gcnew List<Autodesk::DesignScript::Geometry::Surface^>();
		List<Face^>^ pFaces = Faces();
		for each(Face^ pFace in pFaces)
		{
			pDynamoSurfaces->Add(pFace->Surface());
		}

		try {
			return Autodesk::DesignScript::Geometry::Solid::ByJoinedSurfaces(pDynamoSurfaces);
		}
		catch (ApplicationException^)
		{
			return pDynamoSurfaces;
		}
	}

	TopoLogicCore::Topology* Cell::GetCoreTopology()
	{
		assert(m_pCoreCell != nullptr && "Cell::m_pCoreCell is null.");
		if (m_pCoreCell == nullptr)
		{
			throw gcnew Exception("Cell::m_pCoreCell is null.");
		}

		return m_pCoreCell;
	}

	Cell::Cell(Autodesk::DesignScript::Geometry::Solid^ solid)
		: Topology()
		, m_pCoreCell(nullptr)
	{
		if (solid->GetType() == Autodesk::DesignScript::Geometry::Sphere::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (solid->GetType() == Autodesk::DesignScript::Geometry::Cuboid::typeid)
		{
			Init(safe_cast<Autodesk::DesignScript::Geometry::Cuboid^>(solid));
		}
		else if (solid->GetType() == Autodesk::DesignScript::Geometry::Cylinder::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (solid->GetType() == Autodesk::DesignScript::Geometry::Cone::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else
		{
			Autodesk::DesignScript::Geometry::PolySurface^ pDynamoPolysurface = Autodesk
				::DesignScript::Geometry::PolySurface::BySolid(solid);
			List<Face^>^ pFaces = gcnew List<Face^>();
			for each(Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface in pDynamoPolysurface->Surfaces())
			{
				pFaces->Add(gcnew Face(pDynamoSurface));
			}
			Init(pFaces);
		}
	}

	Cell::Cell(List<Face^>^ faces)
		: Topology()
		, m_pCoreCell(nullptr)
	{
		Init(faces);
	}

	Cell::Cell(TopoLogicCore::Cell* const kpCoreCell)
		: Topology()
		, m_pCoreCell(kpCoreCell)
	{

	}

	void Cell::Init(List<Face^>^ faces)
	{
		std::list<TopoLogicCore::Face*> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(pFace->GetCoreTopology()));
		}

		m_pCoreCell = TopoLogicCore::Cell::ByFaces(coreFaces);
	}

	void Cell::Init(Autodesk::DesignScript::Geometry::Cuboid^ cuboid)
	{
		Autodesk::DesignScript::Geometry::Point^ pDynamoCentroid = cuboid->Centroid();
		double length = cuboid->Length;
		double width = cuboid->Width;
		double height = cuboid->Height;
		
		Autodesk::DesignScript::Geometry::CoordinateSystem^ pDynamoCoordinateSystem = cuboid->ContextCoordinateSystem;

		// v1.0: Does not handle custom transformation.
		double xScale = pDynamoCoordinateSystem->XScaleFactor;
		double yScale = pDynamoCoordinateSystem->YScaleFactor;
		double zScale = pDynamoCoordinateSystem->ZScaleFactor;
		double oneMinusPrecision = 1.0 - Precision::Confusion();
		double onePlusPrecision = 1.0 + Precision::Confusion();

		if (xScale < oneMinusPrecision || xScale > onePlusPrecision ||
			yScale < oneMinusPrecision || yScale > onePlusPrecision ||
			zScale < oneMinusPrecision || zScale > onePlusPrecision ||
			!pDynamoCoordinateSystem->XAxis->IsAlmostEqualTo(Autodesk::DesignScript::Geometry::Vector::ByCoordinates(1.0, 0.0, 0.0)) ||
			!pDynamoCoordinateSystem->YAxis->IsAlmostEqualTo(Autodesk::DesignScript::Geometry::Vector::ByCoordinates(0.0, 1.0, 0.0)) || 
			!pDynamoCoordinateSystem->ZAxis->IsAlmostEqualTo(Autodesk::DesignScript::Geometry::Vector::ByCoordinates(0.0, 0.0, 1.0)))
		{
			throw gcnew Exception("Cell creation by cuboid does not currently take transformation.");
		}
		m_pCoreCell = TopoLogicCore::Cell::ByCuboid(
			new Geom_CartesianPoint(pDynamoCentroid->X, pDynamoCentroid->Y, pDynamoCentroid->Z),
			length,
			width,
			height
		);
	}

	List<Face^>^ Cell::Faces()
	{
		TopoLogicCore::Cell* pCoreCell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(GetCoreTopology());
		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreCell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kCoreFaceIterator = coreFaces.begin();
			kCoreFaceIterator != coreFaces.end();
			kCoreFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kCoreFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	Cell::~Cell()
	{
		delete m_pCoreCell;
	}
}