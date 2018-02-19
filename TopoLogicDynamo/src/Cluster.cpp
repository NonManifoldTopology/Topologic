#include <Cluster.h>

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
			coreTopologies.push_back(pTopology->GetCoreTopology());
		}
		
		return gcnew Cluster(TopoLogicCore::Cluster::ByTopology(coreTopologies));
	}

	Cluster^ Cluster::Add(Cluster^ cluster, Topology^ topology)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(cluster->GetCoreTopology());
		pCoreCluster->Add(topology->GetCoreTopology());
		return cluster;
	}

	Cluster^ Cluster::Remove(Cluster^ cluster, Topology^ topology)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(cluster->GetCoreTopology());
		pCoreCluster->Remove(topology->GetCoreTopology());
		return cluster;
	}

	Object^ Cluster::Geometry::get()
	{
		//throw gcnew System::NotImplementedException();
		return nullptr;
	}

	Cluster::Cluster(TopoLogicCore::Cluster* const kpCoreCluster)
		: Topology()
		, m_pCoreCluster(kpCoreCluster)
	{

	}

	TopoLogicCore::Topology* Cluster::GetCoreTopology()
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


	Dictionary<String^, Object^>^ Cluster::Shells(Cluster^ topoLogicCluster)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(topoLogicCluster->GetCoreTopology());

		std::list<TopoLogicCore::Shell*> coreShells;
		pCoreCluster->Shells(coreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Shell*>::const_iterator kShellIterator = coreShells.begin();
			kShellIterator != coreShells.end();
			kShellIterator++)
		{
			Shell^ pShell = gcnew Shell(*kShellIterator);
			pShells->Add(pShell);
			pDynamoEntities->Add(pShell->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Shells", pShells);
		pDictionary->Add("Polysurfaces", pDynamoEntities);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Cluster::Faces(Cluster^ topoLogicCluster)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(topoLogicCluster->GetCoreTopology());

		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreCluster->Faces(coreFaces);

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

	Dictionary<String^, Object^>^ Cluster::Wires(Cluster^ topoLogicCluster)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(topoLogicCluster->GetCoreTopology());

		std::list<TopoLogicCore::Wire*> coreWires;
		pCoreCluster->Wires(coreWires);

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

	Dictionary<String^, Object^>^ Cluster::Edges(Cluster^ topoLogicCluster)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(topoLogicCluster->GetCoreTopology());

		std::list<TopoLogicCore::Edge*> coreEdges;
		pCoreCluster->Edges(coreEdges);

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

	Dictionary<String^, Object^>^ Cluster::Vertices(Cluster^ topoLogicCluster)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(topoLogicCluster->GetCoreTopology());

		std::list<TopoLogicCore::Vertex*> coreVertices;
		pCoreCluster->Vertices(coreVertices);

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

	Dictionary<String^, Object^>^ Cluster::Cells(Cluster^ topoLogicCluster)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(topoLogicCluster->GetCoreTopology());

		std::list<TopoLogicCore::Cell*> coreCells;
		pCoreCluster->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = coreCells.begin();
			kCellIterator != coreCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pCells->Add(pCell);
			pDynamoEntities->Add(pCell->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cells", pCells);
		pDictionary->Add("Solids", pDynamoEntities);
		return pDictionary;
	}
}