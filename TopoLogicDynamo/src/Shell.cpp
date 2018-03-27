#include "Shell.h"
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Cell.h>

#include <assert.h>

namespace TopoLogic
{
	List<Cell^>^ Shell::Cells()
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Cell>> coreCells;
		pCoreShell->Cells(coreCells);

		List<TopoLogic::Cell^>^ pCells = gcnew List<TopoLogic::Cell^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Cell>>::const_iterator kCoreCellIterator = coreCells.begin();
			kCoreCellIterator != coreCells.end();
			kCoreCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCoreCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<Face^>^ Shell::Faces()
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Face>> coreFaces;
		pCoreShell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Face>>::const_iterator kCoreFaceIterator = coreFaces.begin();
			kCoreFaceIterator != coreFaces.end();
			kCoreFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kCoreFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	List<Wire^>^ Shell::Wires()
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Wire>> coreWires;
		pCoreShell->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Wire>>::const_iterator kCoreWireIterator = coreWires.begin();
			kCoreWireIterator != coreWires.end();
			kCoreWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kCoreWireIterator);
			pWires->Add(pWire);
		}

		return pWires;
	}

	List<Edge^>^ Shell::Edges()
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Edge>> coreEdges;
		pCoreShell->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Edge>>::const_iterator kCoreEdgeIterator = coreEdges.begin();
			kCoreEdgeIterator != coreEdges.end();
			kCoreEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kCoreEdgeIterator);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Vertex^>^ Shell::Vertices()
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Vertex>> coreVertices;
		pCoreShell->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Vertex>>::const_iterator kCoreVertexIterator = coreVertices.begin();
			kCoreVertexIterator != coreVertices.end();
			kCoreVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kCoreVertexIterator);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	Shell^ Shell::ByFaces(List<Face^>^ faces)
	{
		std::list<std::shared_ptr<TopoLogicCore::Face>> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Face>(pFace->GetCoreTopologicalQuery()));
		}

		return gcnew Shell(TopoLogicCore::Shell::ByFaces(coreFaces));
	}

	Shell^ Shell::ByPolysurface(Autodesk::DesignScript::Geometry::PolySurface^ polysurface)
	{
		List<Face^>^ pFaces = gcnew List<Face^>();
		for each(Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface in polysurface->Surfaces())
		{
			pFaces->Add(gcnew Face(pDynamoSurface));
		}
		return ByFaces(pFaces);
	}

	Shell^ Shell::ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices)
	{
		std::vector<std::shared_ptr<TopoLogicCore::Vertex>> coreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
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
		
		return gcnew Shell(TopoLogicCore::Shell::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
	}

	Shell^ Shell::ByFacePlanarization(Face^ face, int iteration, int numUPanels, int numVPanels, double tolerance)
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreTopology = TopoLogicCore::Shell::ByFacePlanarization(
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Face>(face->GetCoreTopologicalQuery()),
			iteration,
			numUPanels,
			numVPanels,
			tolerance
		);
		return gcnew Shell(pCoreTopology);
	}

	bool Shell::IsClosed::get()
	{
		std::shared_ptr<TopoLogicCore::Shell> pCoreShell = TopoLogicCore::Topology::Downcast<TopoLogicCore::Shell>(GetCoreTopologicalQuery());
		return pCoreShell->IsClosed();
	}

	Object^ Shell::Geometry::get()
	{
		List<Autodesk::DesignScript::Geometry::Surface^>^ pDynamoSurfaces = gcnew List<Autodesk::DesignScript::Geometry::Surface^>();
		List<Face^>^ pFaces = Faces();
		for each(Face^ pFace in pFaces)
		{
			pDynamoSurfaces->Add(pFace->Surface());
		}

		try {
			return Autodesk::DesignScript::Geometry::PolySurface::ByJoinedSurfaces(pDynamoSurfaces);
		}
		catch (ApplicationException^)
		{
			return pDynamoSurfaces;
		}
	}

	Shell::Shell(const std::shared_ptr<TopoLogicCore::Shell>& kpCoreShell)
		: Topology()
		, m_pCoreShell(new std::shared_ptr<TopoLogicCore::Shell>(kpCoreShell))
	{

	}

	std::shared_ptr<TopoLogicCore::TopologicalQuery> Shell::GetCoreTopologicalQuery()
	{
		assert(m_pCoreShell != nullptr && "Shell::m_pCoreShell is null.");
		if (m_pCoreShell == nullptr)
		{
			throw gcnew Exception("Shell::m_pCoreShell is null.");
		}

		return *m_pCoreShell;
	}

	Shell::~Shell()
	{
		delete m_pCoreShell;
	}
}