#include "Wire.h"
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>

#include <assert.h>

namespace Topologic
{
	List<Edge^>^ Wire::Edges()
	{
		TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Edge::Ptr> pCoreEdgeList;
		pCoreWire->Edges(pCoreEdgeList);
		List<Edge^>^ pEdges = gcnew List<Edge^>();

		for (std::list<TopologicCore::Edge::Ptr>::iterator kCoreEdgeIterator = pCoreEdgeList.begin();
			kCoreEdgeIterator != pCoreEdgeList.end();
			kCoreEdgeIterator++)
		{
			TopologicCore::Edge::Ptr pCoreEdge = *kCoreEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Face^>^ Wire::Faces::get()
	{
		TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Face::Ptr> pCoreFaceList;
		pCoreWire->Faces(pCoreFaceList);
		List<Face^>^ pFaces = gcnew List<Face^>();

		for (std::list<TopologicCore::Face::Ptr>::iterator kCoreFaceIterator = pCoreFaceList.begin();
			kCoreFaceIterator != pCoreFaceList.end();
			kCoreFaceIterator++)
		{
			const TopologicCore::Face::Ptr& kpCoreFace = *kCoreFaceIterator;
			Face^ pFace = gcnew Face(kpCoreFace);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	bool Wire::IsClosed::get()
	{
		return TopologicCore::Topology::Downcast<TopologicCore::Wire>(GetCoreTopologicalQuery())->IsClosed();
	}

	List<Vertex^>^ Wire::Vertices()
	{
		TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Vertex::Ptr> pCoreVertexList;
		pCoreWire->Vertices(pCoreVertexList);
		List<Vertex^>^ pVertices = gcnew List<Vertex^>();

		for (std::list<TopologicCore::Vertex::Ptr>::iterator kCoreVertexIterator = pCoreVertexList.begin();
			kCoreVertexIterator != pCoreVertexList.end();
			kCoreVertexIterator++)
		{
			const TopologicCore::Vertex::Ptr& pCoreVertex = *kCoreVertexIterator;
			Vertex^ pVertex = gcnew Vertex(pCoreVertex);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	int Wire::GetNumberOfBranches()
	{
		TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(GetCoreTopologicalQuery());
		return pCoreWire->GetNumberOfBranches();
	}

	Wire^ Wire::ByEdges(System::Collections::Generic::IEnumerable<Edge^>^ edges)
	{
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		for each(Edge^ pEdge in edges)
		{
			coreEdges.push_back(TopologicCore::Topology::Downcast<TopologicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		try {
			return gcnew Wire(TopologicCore::Wire::ByEdges(coreEdges));
		}
		catch (const std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}

	Wire^ Wire::ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve ^ polycurve)
	{
		array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurves = polycurve->Curves();

		std::list<TopologicCore::Edge::Ptr> coreEdges;
		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
		{
			Edge^ pEdge = Edge::ByCurve(pDynamoCurve);
			coreEdges.push_back(TopologicCore::Topology::Downcast<TopologicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		Exception^ e = nullptr;
		TopologicCore::Wire::Ptr pCoreWire = nullptr;
		try{
			pCoreWire = TopologicCore::Wire::ByEdges(coreEdges);
		}
		catch (const std::exception& rkException)
		{
			e = gcnew Exception(gcnew String(rkException.what()));
		}

		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
		{
			delete pDynamoCurve;
		}
		
		if (e != nullptr)
		{
			throw e;
		}

		return gcnew Wire(pCoreWire);
	}

	Object^ Wire::Geometry::get()
	{
		List<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurves = gcnew List<Autodesk::DesignScript::Geometry::Curve^>();
		List<Edge^>^ pEdges = Edges();
		for each(Edge^ pEdge in pEdges)
		{
			pDynamoCurves->Add(pEdge->Curve());
		}

		if (pDynamoCurves->Count == 0)
			return nullptr;

		List<Object^>^ pDynamoGeometries = gcnew List<Object^>();
		try
		{
			pDynamoGeometries->Add(Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurves, 0.001));

			for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
			{
				delete pDynamoCurve;
			}
		}
		catch (std::exception&)
		{
			for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
			{
				pDynamoGeometries->Add(pDynamoCurve);
			}
		}
		catch (Exception^)
		{
			for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
			{
				pDynamoGeometries->Add(pDynamoCurve);
			}
		}
		catch (...)
		{
			throw gcnew Exception("An unknown Wire::Geometry exception is encountered.");
		}

		Object^ objColoredSubcontents = Topology::Geometry::get();
		List<Object^>^ coloredSubcontents = dynamic_cast<List<Object^>^>(objColoredSubcontents);
		pDynamoGeometries->AddRange(coloredSubcontents);
		return CleanupGeometryOutput(pDynamoGeometries);
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Wire::GetCoreTopologicalQuery()
	{
		assert(m_pCoreWire != nullptr && "Wire::m_pCoreWire is null.");
		if (m_pCoreWire == nullptr)
		{
			throw gcnew Exception("Wire::m_pCoreWire is null.");
		}

		return *m_pCoreWire;
	}

	int Wire::Type()
	{
		return TopologicCore::Wire::Type();
	}

	Wire::Wire(const TopologicCore::Wire::Ptr& kpCoreWire)
		: Topology()
		, m_pCoreWire(kpCoreWire != nullptr? new TopologicCore::Wire::Ptr(kpCoreWire) : throw gcnew Exception("A null wire was created."))
	{

	}

	Wire::~Wire()
	{
		delete m_pCoreWire;
	}
}