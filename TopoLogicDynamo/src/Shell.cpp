#include "Shell.h"
#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Cell.h>

#include <assert.h>

namespace Topologic
{
	List<Cell^>^ Shell::Cells(Topology^ parentTopology)
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

		std::list<TopologicCore::Cell::Ptr> coreCells;
		pCoreShell->Cells(pCoreParentTopology, coreCells);

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

	List<Face^>^ Shell::Faces()
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

	List<Wire^>^ Shell::Wires()
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

	List<Edge^>^ Shell::Edges()
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

	List<Vertex^>^ Shell::Vertices()
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

	Shell^ Shell::ByFaces(List<Face^>^ faces)
	{
		std::list<TopologicCore::Face::Ptr> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopologicCore::Topology::Downcast<TopologicCore::Face>(pFace->GetCoreTopologicalQuery()));
		}

		return gcnew Shell(TopologicCore::Shell::ByFaces(coreFaces));
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

	Shell^ Shell::ByLoft(List<Wire^>^ wires)
	{
		std::list<TopologicCore::Wire::Ptr> coreWires;
		for each(Wire^ pWire in wires)
		{
			coreWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
		}

		try {
			TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Shell::ByLoft(coreWires);
			return gcnew Shell(pCoreShell);
		}
		catch (std::exception&)
		{
			throw gcnew Exception("Loft error");
		}
	}

	Shell^ Shell::ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices)
	{
		std::vector<TopologicCore::Vertex::Ptr> coreVertices;
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
		
		return gcnew Shell(TopologicCore::Shell::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
	}

	List<Face^>^ Shell::ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, int numUPanels, int numVPanels, double tolerance, bool capBottom, bool capTop)
	{
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		std::list<TopologicCore::Edge::Ptr> coreIsocurves;
		std::list<TopologicCore::Wire::Ptr> coreWires;
		std::list<TopologicCore::Face::Ptr> coreFaces;
		TopologicCore::Shell::Ptr pCoreTopology = TopologicCore::Shell::ByFacePlanarization(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery()),
			iteration,
			numEdgeSamples,
			numUPanels,
			numVPanels,
			tolerance,
			capBottom,
			capTop,
			coreVertices,
			coreIsocurves,
			coreWires,
			coreFaces
		);
		//return gcnew Shell(pCoreTopology);

		List<Edge^>^ isoedges = gcnew List<Edge^>();
		for (const TopologicCore::Edge::Ptr& rkCoreEdge : coreIsocurves)
		{
			isoedges->Add(safe_cast<Edge^>(Topology::ByCoreTopology(rkCoreEdge)));
		}
		List<Face^>^ faces = gcnew List<Face^>();
		for(const TopologicCore::Face::Ptr& rkCoreFace : coreFaces)
		{
			faces->Add(safe_cast<Face^>(Topology::ByCoreTopology(rkCoreFace)));
		}
		return faces;
	}

	Dictionary<String^, Object^>^ Shell::ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, List<double>^ uValues, List<double>^ vValues, double tolerance, bool capBottom, bool capTop)
	{
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		std::list<TopologicCore::Edge::Ptr> coreIsocurves;
		std::list<TopologicCore::Wire::Ptr> coreWires;
		std::list<TopologicCore::Face::Ptr> coreFaces;
		std::list<double> coreUValues, coreVValues;
		for each(double u in uValues)
		{
			coreUValues.push_back(u);
		}

		for each(double v in vValues)
		{
			coreVValues.push_back(v);
		}

		TopologicCore::Shell::Ptr pCoreTopology = TopologicCore::Shell::ByFacePlanarization(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Face>(face->GetCoreTopologicalQuery()),
			iteration,
			numEdgeSamples,
			coreUValues,
			coreVValues,
			tolerance,
			capBottom,
			capTop,
			coreVertices,
			coreIsocurves,
			coreWires,
			coreFaces
		);
		Shell^ pShell = gcnew Shell(pCoreTopology);
		//return gcnew Shell(pCoreTopology);

		List<Vertex^>^ vertices = gcnew List<Vertex^>();
		for (const TopologicCore::Vertex::Ptr& rkCoreVertex : coreVertices)
		{
			vertices->Add(safe_cast<Vertex^>(Topology::ByCoreTopology(rkCoreVertex)));
		}
		List<Edge^>^ edges = gcnew List<Edge^>();
		for (const TopologicCore::Edge::Ptr& rkCoreEdge : coreIsocurves)
		{
			edges->Add(safe_cast<Edge^>(Topology::ByCoreTopology(rkCoreEdge)));
		}
		List<Wire^>^ wires = gcnew List<Wire^>();
		for (const TopologicCore::Wire::Ptr& rkCoreWire : coreWires)
		{
			wires->Add(safe_cast<Wire^>(Topology::ByCoreTopology(rkCoreWire)));
		}
		List<Face^>^ faces = gcnew List<Face^>();
		for (const TopologicCore::Face::Ptr& rkCoreFace : coreFaces)
		{
			faces->Add(safe_cast<Face^>(Topology::ByCoreTopology(rkCoreFace)));
		}
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("Vertices", vertices);
		pDictionary->Add("Edges", edges);
		pDictionary->Add("Wires", wires);
		pDictionary->Add("Faces", faces);
		pDictionary->Add("Shell", pShell);
		return pDictionary;
		//return faces;
	}

	bool Shell::IsClosed::get()
	{
		TopologicCore::Shell::Ptr pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
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
			Autodesk::DesignScript::Geometry::PolySurface^ pDynamoPolySurface =
				Autodesk::DesignScript::Geometry::PolySurface::ByJoinedSurfaces(pDynamoSurfaces);

			return pDynamoPolySurface;
		}
		catch (ApplicationException^)
		{
			return pDynamoSurfaces;
		}
	}

	Shell::Shell(const TopologicCore::Shell::Ptr& kpCoreShell)
		: Topology()
		, m_pCoreShell(new TopologicCore::Shell::Ptr(kpCoreShell))
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
		//delete m_pCoreShell;
	}
}