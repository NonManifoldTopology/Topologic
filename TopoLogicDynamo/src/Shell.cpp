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
		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Cell>> coreCells;
		pCoreShell->Cells(pCoreParentTopology, coreCells);

		List<Topologic::Cell^>^ pCells = gcnew List<Topologic::Cell^>();
		for (std::list<std::shared_ptr<TopologicCore::Cell>>::const_iterator kCoreCellIterator = coreCells.begin();
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
		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Face>> coreFaces;
		pCoreShell->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<std::shared_ptr<TopologicCore::Face>>::const_iterator kCoreFaceIterator = coreFaces.begin();
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
		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Wire>> coreWires;
		pCoreShell->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<std::shared_ptr<TopologicCore::Wire>>::const_iterator kCoreWireIterator = coreWires.begin();
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
		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Edge>> coreEdges;
		pCoreShell->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<std::shared_ptr<TopologicCore::Edge>>::const_iterator kCoreEdgeIterator = coreEdges.begin();
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
		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Vertex>> coreVertices;
		pCoreShell->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<std::shared_ptr<TopologicCore::Vertex>>::const_iterator kCoreVertexIterator = coreVertices.begin();
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
		std::list<std::shared_ptr<TopologicCore::Face>> coreFaces;
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
		std::list<std::shared_ptr<TopologicCore::Wire>> coreWires;
		for each(Wire^ pWire in wires)
		{
			coreWires.push_back(TopologicCore::Topology::Downcast<TopologicCore::Wire>(pWire->GetCoreTopologicalQuery()));
		}

		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Shell::ByLoft(coreWires);
		return gcnew Shell(pCoreShell);
	}

	Shell^ Shell::ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices)
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
		
		return gcnew Shell(TopologicCore::Shell::ByVerticesFaceIndices(coreVertices, coreFaceIndices));
	}

	List<Face^>^ Shell::ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, int numUPanels, int numVPanels, double tolerance, bool capBottom, bool capTop)
	{
		std::list<std::shared_ptr<TopologicCore::Vertex>> coreVertices;
		std::list<std::shared_ptr<TopologicCore::Edge>> coreIsocurves;
		std::list<std::shared_ptr<TopologicCore::Wire>> coreWires;
		std::list<std::shared_ptr<TopologicCore::Face>> coreFaces;
		std::shared_ptr<TopologicCore::Shell> pCoreTopology = TopologicCore::Shell::ByFacePlanarization(
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
		for (const std::shared_ptr<TopologicCore::Edge>& rkCoreEdge : coreIsocurves)
		{
			isoedges->Add(safe_cast<Edge^>(Topology::ByCoreTopology(rkCoreEdge)));
		}
		List<Face^>^ faces = gcnew List<Face^>();
		for(const std::shared_ptr<TopologicCore::Face>& rkCoreFace : coreFaces)
		{
			faces->Add(safe_cast<Face^>(Topology::ByCoreTopology(rkCoreFace)));
		}
		return faces;
	}

	Dictionary<String^, Object^>^ Shell::ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, List<double>^ uValues, List<double>^ vValues, double tolerance, bool capBottom, bool capTop)
	{
		std::list<std::shared_ptr<TopologicCore::Vertex>> coreVertices;
		std::list<std::shared_ptr<TopologicCore::Edge>> coreIsocurves;
		std::list<std::shared_ptr<TopologicCore::Wire>> coreWires;
		std::list<std::shared_ptr<TopologicCore::Face>> coreFaces;
		std::list<double> coreUValues, coreVValues;
		for each(double u in uValues)
		{
			coreUValues.push_back(u);
		}

		for each(double v in vValues)
		{
			coreVValues.push_back(v);
		}

		std::shared_ptr<TopologicCore::Shell> pCoreTopology = TopologicCore::Shell::ByFacePlanarization(
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
		for (const std::shared_ptr<TopologicCore::Vertex>& rkCoreVertex : coreVertices)
		{
			vertices->Add(safe_cast<Vertex^>(Topology::ByCoreTopology(rkCoreVertex)));
		}
		List<Edge^>^ edges = gcnew List<Edge^>();
		for (const std::shared_ptr<TopologicCore::Edge>& rkCoreEdge : coreIsocurves)
		{
			edges->Add(safe_cast<Edge^>(Topology::ByCoreTopology(rkCoreEdge)));
		}
		List<Wire^>^ wires = gcnew List<Wire^>();
		for (const std::shared_ptr<TopologicCore::Wire>& rkCoreWire : coreWires)
		{
			wires->Add(safe_cast<Wire^>(Topology::ByCoreTopology(rkCoreWire)));
		}
		List<Face^>^ faces = gcnew List<Face^>();
		for (const std::shared_ptr<TopologicCore::Face>& rkCoreFace : coreFaces)
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
		std::shared_ptr<TopologicCore::Shell> pCoreShell = TopologicCore::Topology::Downcast<TopologicCore::Shell>(GetCoreTopologicalQuery());
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

	Shell::Shell(const std::shared_ptr<TopologicCore::Shell>& kpCoreShell)
		: Topology()
		, m_pCoreShell(new std::shared_ptr<TopologicCore::Shell>(kpCoreShell))
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