#include "Shell.h"
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Cell.h>

#include <assert.h>

namespace Topologic
{
	List<Cell^>^ Shell::Cells::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Cell::Ptr> coreCells;
		pCoreShell->Cells(coreCells);

		List<Topologic::Cell^>^ pCells = gcnew List<Topologic::Cell^>();
		for (std::list<TopologicCore::Cell::Ptr>::const_iterator kCoreCellIterator = coreCells.begin();
			kCoreCellIterator != coreCells.end();
			kCoreCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCoreCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<Face^>^ Shell::Faces::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Face::Ptr> coreFaces;
		pCoreShell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopologicCore::Face::Ptr>::const_iterator kCoreFaceIterator = coreFaces.begin();
			kCoreFaceIterator != coreFaces.end();
			kCoreFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kCoreFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	List<Wire^>^ Shell::Wires::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Wire::Ptr> coreWires;
		pCoreShell->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopologicCore::Wire::Ptr>::const_iterator kCoreWireIterator = coreWires.begin();
			kCoreWireIterator != coreWires.end();
			kCoreWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kCoreWireIterator);
			pWires->Add(pWire);
		}

		return pWires;
	}

	List<Edge^>^ Shell::Edges::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		pCoreShell->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopologicCore::Edge::Ptr>::const_iterator kCoreEdgeIterator = coreEdges.begin();
			kCoreEdgeIterator != coreEdges.end();
			kCoreEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kCoreEdgeIterator);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Vertex^>^ Shell::Vertices::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		pCoreShell->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopologicCore::Vertex::Ptr>::const_iterator kCoreVertexIterator = coreVertices.begin();
			kCoreVertexIterator != coreVertices.end();
			kCoreVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kCoreVertexIterator);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	Shell^ Shell::ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces, double tolerance)
	{
		std::list<TopologicCore::Face::Ptr> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopologicCore::Topology::Downcast<TopologicCore::Face>(pFace->GetCoreTopologicalQuery()));
		}

		TopologicCore::Shell::Ptr pCoreShell = nullptr;
		try {
			pCoreShell = TopologicCore::Shell::ByFaces(coreFaces, tolerance);
		}
		catch (const std::exception& rkException)
		{
			throw gcnew Exception(gcnew String(rkException.what()));
		}

		return gcnew Shell(pCoreShell);
	}

	bool Shell::IsClosed::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		return pCoreShell->IsClosed();
	}

	Object^ Shell::BasicGeometry::get()
	{
#ifdef TOPOLOGIC_DYNAMO
		List<Autodesk::DesignScript::Geometry::Surface^>^ pDynamoSurfaces = gcnew List<Autodesk::DesignScript::Geometry::Surface^>();
		List<Face^>^ pFaces = Faces;
		for each(Face^ pFace in pFaces)
		{
			pDynamoSurfaces->Add(pFace->Surface());
		}

		try {
			Autodesk::DesignScript::Geometry::PolySurface^ pDynamoPolySurface =
				Autodesk::DesignScript::Geometry::PolySurface::ByJoinedSurfaces(pDynamoSurfaces);

			return pDynamoPolySurface;
		}
		catch (ApplicationException^)
		{
			return pDynamoSurfaces;
		}
#else
		return nullptr;
#endif
	}

	int Shell::Type()
	{
		return TopologicCore::Shell::Type();
	}

	Shell::Shell(const TopologicCore::Shell::Ptr& kpCoreShell)
		: Topology()
		, m_pCoreShell(kpCoreShell != nullptr ? new TopologicCore::Shell::Ptr(kpCoreShell) : throw gcnew Exception("A null shell was created."))
	{

	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Shell::GetCoreTopologicalQuery()
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