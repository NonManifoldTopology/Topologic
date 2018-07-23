#include "Cluster.h"

#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <Cell.h>
#include <CellComplex.h>

#include <assert.h>

namespace Topologic
{
	Cluster^ Cluster::ByTopologies(IEnumerable<Topology^>^ topology)
	{
		std::list<std::shared_ptr<TopologicCore::Topology>> coreTopologies;
		for each(Topology^ pTopology in topology)
		{
			std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(pTopology->GetCoreTopologicalQuery());
			if(pCoreTopology != nullptr)
			{
				coreTopologies.push_back(pCoreTopology);
			}
		}
		
		return gcnew Cluster(TopologicCore::Cluster::ByTopologies(coreTopologies));
	}

	Cluster^ Cluster::AddTopology(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		if (!pCoreCluster->AddTopology(pCoreTopology.get()))
		{
			throw gcnew Exception("Cluster::Add(): fails to add topology");
		}
		return this;
	}

	Cluster^ Cluster::RemoveTopology(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());
		if (!pCoreCluster->RemoveTopology(pCoreTopology.get()))
		{
			throw gcnew Exception("Cluster::Remove(): fails to remove topology");
		}
		return this;
	}

	Object^ Cluster::Geometry::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Topology>> immediateCoreMembers;
		pCoreCluster->ImmediateMembers(immediateCoreMembers);

		List<Object^>^ pTopologies = gcnew List<Object^>();

		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreIterator = immediateCoreMembers.begin();
			kCoreIterator != immediateCoreMembers.end();
			kCoreIterator++)
		{
			pTopologies->Add(Topology::ByCoreTopology(*kCoreIterator)->Geometry);
		}
		return pTopologies;
	}

	Cluster::Cluster(const TopologicCore::Cluster::Ptr& kpCoreCluster)
		: Topology()
		, m_pCoreCluster(new TopologicCore::Cluster::Ptr(kpCoreCluster))
	{

	}

	Cluster::Cluster()
		: Topology()
		, m_pCoreCluster(nullptr)
	{
		
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Cluster::GetCoreTopologicalQuery()
	{
		assert(m_pCoreCluster != nullptr && "Cluster::m_pCoreCluster is null.");
		if (m_pCoreCluster == nullptr)
		{
			throw gcnew Exception("Cluster::m_pCoreCluster is null.");
		}

		return *m_pCoreCluster;
	}

	Cluster::~Cluster()
	{
		//delete m_pCoreCluster;
	}


	List<Shell^>^ Cluster::Shells()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Shell::Ptr> coreShells;
		pCoreCluster->Shells(coreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		for (std::list<TopologicCore::Shell::Ptr>::const_iterator kShellIterator = coreShells.begin();
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
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Face::Ptr> coreFaces;
		pCoreCluster->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopologicCore::Face::Ptr>::const_iterator kFaceIterator = coreFaces.begin();
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
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Wire::Ptr> coreWires;
		pCoreCluster->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopologicCore::Wire::Ptr>::const_iterator kWireIterator = coreWires.begin();
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
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Edge::Ptr> coreEdges;
		pCoreCluster->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopologicCore::Edge::Ptr>::const_iterator kEdgeIterator = coreEdges.begin();
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
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		pCoreCluster->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopologicCore::Vertex::Ptr>::const_iterator kVertexIterator = coreVertices.begin();
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
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Cell::Ptr> coreCells;
		pCoreCluster->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopologicCore::Cell::Ptr>::const_iterator kCellIterator = coreCells.begin();
			kCellIterator != coreCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<CellComplex^>^ Cluster::CellComplexes()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::CellComplex::Ptr> coreCellComplexes;
		pCoreCluster->CellComplexes(coreCellComplexes);

		List<CellComplex^>^ pCellComplexes = gcnew List<CellComplex^>();
		for (std::list<TopologicCore::CellComplex::Ptr>::const_iterator kCellComplexIterator = coreCellComplexes.begin();
			kCellComplexIterator != coreCellComplexes.end();
			kCellComplexIterator++)
		{
			CellComplex^ pCellComplex = gcnew CellComplex(*kCellComplexIterator);
			pCellComplexes->Add(pCellComplex);
		}

		return pCellComplexes;
	}

}