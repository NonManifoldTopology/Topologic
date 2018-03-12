#include "Cluster.h"

#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <Cell.h>
#include <CellComplex.h>

#include <assert.h>

namespace TopoLogic
{
	Cluster^ Cluster::ByTopology(List<Topology^>^ topology)
	{
		std::list<TopoLogicCore::Topology*> coreTopologies;
		for each(Topology^ pTopology in topology)
		{
			TopoLogicCore::Topology* pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(pTopology->GetCoreTopologicalQuery());
			if(pCoreTopology != nullptr)
			{
				coreTopologies.push_back(pCoreTopology);
			}
		}
		
		return gcnew Cluster(TopoLogicCore::Cluster::ByTopology(coreTopologies));
	}

	Cluster^ Cluster::Add(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		if (!pCoreCluster->Add(pCoreTopology))
		{
			throw gcnew Exception("Cluster::Add(): fails to add topology");
		}
		return this;
	}

	Cluster^ Cluster::Remove(Topology^ topology)
	{
		TopoLogicCore::Topology* pCoreTopology = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());
		if (!pCoreCluster->Remove(pCoreTopology))
		{
			throw gcnew Exception("Cluster::Remove(): fails to remove topology");
		}
		return this;
	}

	Object^ Cluster::Geometry::get()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());
		std::list<TopoLogicCore::Topology*> immediateCoreMembers;
		pCoreCluster->ImmediateMembers(immediateCoreMembers);

		List<Object^>^ pTopologies = gcnew List<Object^>();

		for (std::list<TopoLogicCore::Topology*>::const_iterator kCoreIterator = immediateCoreMembers.begin();
			kCoreIterator != immediateCoreMembers.end();
			kCoreIterator++)
		{
			pTopologies->Add(Topology::ByCoreTopology(*kCoreIterator)->Geometry);
		}
		return pTopologies;
	}

	Cluster::Cluster(TopoLogicCore::Cluster* const kpCoreCluster)
		: Topology()
		, m_pCoreCluster(kpCoreCluster)
	{

	}

	TopoLogicCore::TopologicalQuery* Cluster::GetCoreTopologicalQuery()
	{
		assert(m_pCoreCluster != nullptr && "Cluster::m_pCoreCluster is null.");
		if (m_pCoreCluster == nullptr)
		{
			throw gcnew Exception("Cluster::m_pCoreCluster is null.");
		}

		return m_pCoreCluster;
	}

	Cluster::~Cluster()
	{
		delete m_pCoreCluster;
	}


	List<Shell^>^ Cluster::Shells()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Shell*> coreShells;
		pCoreCluster->Shells(coreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		for (std::list<TopoLogicCore::Shell*>::const_iterator kShellIterator = coreShells.begin();
			kShellIterator != coreShells.end();
			kShellIterator++)
		{
			Shell^ pShell = gcnew Shell(*kShellIterator);
			pShells->Add(pShell);
		}

		return pShells;
	}

	List<Face^>^ Cluster::Faces()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreCluster->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kFaceIterator = coreFaces.begin();
			kFaceIterator != coreFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	List<Wire^>^ Cluster::Wires()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Wire*> coreWires;
		pCoreCluster->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopoLogicCore::Wire*>::const_iterator kWireIterator = coreWires.begin();
			kWireIterator != coreWires.end();
			kWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kWireIterator);
			pWires->Add(pWire);
		}

		return pWires;
	}

	List<Edge^>^ Cluster::Edges()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Edge*> coreEdges;
		pCoreCluster->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = coreEdges.begin();
			kEdgeIterator != coreEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Vertex^>^ Cluster::Vertices()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Vertex*> coreVertices;
		pCoreCluster->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = coreVertices.begin();
			kVertexIterator != coreVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	List<Cell^>^ Cluster::Cells()
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Cell*> coreCells;
		pCoreCluster->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = coreCells.begin();
			kCellIterator != coreCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}
}