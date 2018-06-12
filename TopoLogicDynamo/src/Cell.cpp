#include "Cell.h"
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <CellComplex.h>

#include <Geom_CartesianPoint.hxx>
#include <Precision.hxx>

#include <assert.h>

namespace Topologic
{
	Cell^ Cell::ByFaces(List<Face^>^ faces)
	{
		return gcnew Cell(faces);
	}

	Cell^ Cell::BySolid(Autodesk::DesignScript::Geometry::Solid^ solid)
	{
		return gcnew Cell(solid);
	}

	Cell^ Cell::ByShell(Shell^ shell)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Cell::ByShell(TopologicCore::Topology::Downcast<TopologicCore::Shell>(shell->GetCoreTopologicalQuery()));
		return gcnew Cell(pCoreCell);
	}

	Cell^ Cell::ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices)
	{
		std::vector<std::shared_ptr<TopologicCore::Vertex>> coreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			coreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
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

		Cell^ pCell = gcnew Cell(TopologicCore::Cell::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
		return pCell;
	}

	Cell^ Cell::ByLoft(List<Wire^>^ wires)
	{

		std::list<std::shared_ptr<TopologicCore::Wire>> coreWires;
		for each(Wire^ pWire in wires)
		{
			coreWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
		}

		try {
			std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Cell::ByLoft(coreWires);
			return gcnew Cell(pCoreCell);
		}
		catch (std::exception&)
		{
			throw gcnew Exception("Loft error");
		}
	}

	List<CellComplex^>^ Cell::CellComplexes(Topology^ parentTopology)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::CellComplex>> coreCellComplexes;
		pCoreCell->CellComplexes(pCoreParentTopology, coreCellComplexes);

		List<CellComplex^>^ pCellComplexes = gcnew List<CellComplex^>();
		for (std::list<std::shared_ptr<TopologicCore::CellComplex>>::const_iterator kCellComplexIterator = coreCellComplexes.begin();
			kCellComplexIterator != coreCellComplexes.end();
			kCellComplexIterator++)
		{
			CellComplex^ pCellComplex = gcnew CellComplex(*kCellComplexIterator);
			pCellComplexes->Add(pCellComplex);
		}

		return pCellComplexes;
	}

	List<Shell^>^ Cell::Shells()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Shell>> coreShells;
		pCoreCell->Shells(coreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		for (std::list<std::shared_ptr<TopologicCore::Shell>>::const_iterator kShellIterator = coreShells.begin();
			kShellIterator != coreShells.end();
			kShellIterator++)
		{
			Shell^ pShell = gcnew Shell(*kShellIterator);
			pShells->Add(pShell);
		}

		return pShells;
	}

	List<Face^>^ Cell::Faces()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Face>> coreFaces;
		pCoreCell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<std::shared_ptr<TopologicCore::Face>>::const_iterator kFaceIterator = coreFaces.begin();
			kFaceIterator != coreFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	List<Wire^>^ Cell::Wires()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Wire>> coreWires;
		pCoreCell->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<std::shared_ptr<TopologicCore::Wire>>::const_iterator kWireIterator = coreWires.begin();
			kWireIterator != coreWires.end();
			kWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kWireIterator);
			pWires->Add(pWire);
		}

		return pWires;
	}

	List<Edge^>^ Cell::Edges()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Edge>> coreEdges;
		pCoreCell->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<std::shared_ptr<TopologicCore::Edge>>::const_iterator kEdgeIterator = coreEdges.begin();
			kEdgeIterator != coreEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Vertex^>^ Cell::Vertices()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Vertex>> coreVertices;
		pCoreCell->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<std::shared_ptr<TopologicCore::Vertex>>::const_iterator kVertexIterator = coreVertices.begin();
			kVertexIterator != coreVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	List<Cell^>^ Cell::AdjacentCells(Cell^ cell)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Cell>> coreAdjacentCells;
		pCoreCell->AdjacentCells(coreAdjacentCells);

		List<Cell^>^ pAdjacentCells = gcnew List<Cell^>();
		for (std::list<std::shared_ptr<TopologicCore::Cell>>::const_iterator kCellIterator = coreAdjacentCells.begin();
			kCellIterator != coreAdjacentCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pAdjacentCells->Add(pCell);
		}

		return pAdjacentCells;
	}

	List<Face^>^ Cell::SharedFaces(Cell^ cell)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell1 = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Cell> pCoreCell2 = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Face>> sharedFaces;
		pCoreCell1->SharedFaces(pCoreCell2, sharedFaces);

		List<Face^>^ pSharedFaces = gcnew List<Face^>();
		for(std::list<std::shared_ptr<TopologicCore::Face>>::const_iterator kFaceIterator = sharedFaces.begin();
			kFaceIterator != sharedFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pSharedFaces->Add(pFace);
		}

		return pSharedFaces;
	}

	List<Edge^>^ Cell::SharedEdges(Cell^ cell)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell1 = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Cell> pCoreCell2 = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Edge>> sharedEdges;
		pCoreCell1->SharedEdges(pCoreCell2, sharedEdges);

		List<Edge^>^ pSharedEdges = gcnew List<Edge^>();
		for (std::list<std::shared_ptr<TopologicCore::Edge>>::const_iterator kEdgeIterator = sharedEdges.begin();
			kEdgeIterator != sharedEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pSharedEdges->Add(pEdge);
		}

		return pSharedEdges;
	}

	List<Vertex^>^ Cell::SharedVertices(Cell^ cell)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell1 = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Cell> pCoreCell2 = TopologicCore::Topology::Downcast<TopologicCore::Cell>(cell->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Vertex>> sharedVertices;
		pCoreCell1->SharedVertices(pCoreCell2, sharedVertices);

		List<Vertex^>^ pSharedVertices = gcnew List<Vertex^>();
		for (std::list<std::shared_ptr<TopologicCore::Vertex>>::const_iterator kVertexIterator = sharedVertices.begin();
			kVertexIterator != sharedVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pSharedVertices->Add(pVertex);
		}

		return pSharedVertices;
	}

	Shell^ Cell::OuterBoundary()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		return gcnew Shell(pCoreCell->OuterBoundary());
	}

	List<Shell^>^ Cell::InnerBoundaries()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Shell>> coreInnerShells;
		pCoreCell->InnerBoundaries(coreInnerShells);

		List<Shell^>^ pInnerShells = gcnew List<Shell^>();
		for (std::list<std::shared_ptr<TopologicCore::Shell>>::const_iterator kShellIterator = coreInnerShells.begin();
			kShellIterator != coreInnerShells.end();
			kShellIterator++)
		{
			Shell^ pShell = gcnew Shell(*kShellIterator);
			pInnerShells->Add(pShell);
		}

		return pInnerShells;
	}

	double Cell::Volume()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		return pCoreCell->Volume();
	}

	Vertex^ Cell::CenterOfMass()
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Vertex> pCoreCenterOfMass = pCoreCell->CenterOfMass();
		return gcnew Vertex(pCoreCenterOfMass);
	}

	bool Cell::DoesContain(Vertex ^ vertex)
	{
		std::shared_ptr<TopologicCore::Cell> pCoreCell = TopologicCore::Topology::Downcast<TopologicCore::Cell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Vertex> pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery());
		return pCoreCell->DoesContain(pCoreVertex);
	}

	Object^ Cell::Geometry::get()
	{
		List<Autodesk::DesignScript::Geometry::Surface^>^ pDynamoSurfaces = gcnew List<Autodesk::DesignScript::Geometry::Surface^>();
		List<Object^>^ pDynamoGeometries = gcnew List<Object^>();
		List<Face^>^ pFaces = Faces();
		bool hasFallbackVisualization = false;
		for each(Face^ pFace in pFaces)
		{
			Object^ pFaceGeometry = pFace->Geometry;
			Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface = dynamic_cast<Autodesk::DesignScript::Geometry::Surface^>(pFaceGeometry);
			pDynamoGeometries->Add(pFaceGeometry);
			if(pDynamoSurface != nullptr)
			{
				pDynamoSurfaces->Add(pDynamoSurface);
			}
			else
			{
				// We are getting a null geometry! This is possible when the face gives a Mesh (i.e. not a Surface) as its geometry representation.
				hasFallbackVisualization = true;
			}
		}

		if(!hasFallbackVisualization)
		{
			try {
				if (!pDynamoSurfaces->Contains(nullptr))
				{
					Autodesk::DesignScript::Geometry::Solid^ pDynamoSolid = Autodesk::DesignScript::Geometry::Solid::ByJoinedSurfaces(pDynamoSurfaces);
					for each(Object^ pDynamoGeometry in pDynamoGeometries)
					{
						delete pDynamoGeometry;
					}
					return pDynamoSolid;
				}
				else
				{
					return pDynamoSurfaces;
				}
			}
			catch (Exception^)
			{
			}
		}

		return pDynamoGeometries;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Cell::GetCoreTopologicalQuery()
	{
		assert(m_pCoreCell != nullptr && "Cell::m_pCoreCell is null.");
		if (m_pCoreCell == nullptr)
		{
			throw gcnew Exception("Cell::m_pCoreCell is null.");
		}

		return *m_pCoreCell;
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
				delete pDynamoSurface;
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

	Cell::Cell(const std::shared_ptr<TopologicCore::Cell>& kpCoreCell)
		: Topology()
		, m_pCoreCell(new std::shared_ptr<TopologicCore::Cell>(kpCoreCell))
	{

	}

	void Cell::Init(List<Face^>^ faces)
	{
		std::list<std::shared_ptr<TopologicCore::Face>> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopologicCore::Topology::Downcast<TopologicCore::Face>(pFace->GetCoreTopologicalQuery()));
		}

		m_pCoreCell = new std::shared_ptr<TopologicCore::Cell>(TopologicCore::Cell::ByFaces(coreFaces));
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

		Autodesk::DesignScript::Geometry::Vector^ pDynamoXAxis = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(1.0, 0.0, 0.0);
		Autodesk::DesignScript::Geometry::Vector^ pDynamoYAxis = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(0.0, 1.0, 0.0);
		Autodesk::DesignScript::Geometry::Vector^ pDynamoZAxis = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(0.0, 0.0, 1.0);

		bool canCreateCell = true;
		if (xScale < oneMinusPrecision || xScale > onePlusPrecision ||
			yScale < oneMinusPrecision || yScale > onePlusPrecision ||
			zScale < oneMinusPrecision || zScale > onePlusPrecision ||
			!pDynamoCoordinateSystem->XAxis->IsAlmostEqualTo(pDynamoXAxis) ||
			!pDynamoCoordinateSystem->YAxis->IsAlmostEqualTo(pDynamoYAxis) || 
			!pDynamoCoordinateSystem->ZAxis->IsAlmostEqualTo(pDynamoZAxis))
		{
			canCreateCell = false; 
		}

		if(canCreateCell)
		{
			m_pCoreCell = new std::shared_ptr<TopologicCore::Cell>(TopologicCore::Cell::ByCuboid(
				new Geom_CartesianPoint(pDynamoCentroid->X, pDynamoCentroid->Y, pDynamoCentroid->Z),
				width,
				length,
				height
			));
		}

		delete pDynamoCentroid;
		delete pDynamoCoordinateSystem;
		delete pDynamoXAxis;
		delete pDynamoYAxis;
		delete pDynamoZAxis;

		if (!canCreateCell)
		{
			throw gcnew Exception("Cell creation by cuboid does not currently take transformation.");
		}
	}

	Cell::~Cell()
	{
		//delete m_pCoreCell;
	}
}