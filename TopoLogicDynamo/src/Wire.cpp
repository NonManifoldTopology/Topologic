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

	List<Face^>^ Wire::Faces(Topology^ parentTopology)
	{
		TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());
		std::list<TopologicCore::Face::Ptr> pCoreFaceList;
		pCoreWire->Faces(pCoreParentTopology, pCoreFaceList);
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

	bool Wire::IsClosed()
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

	Wire^ Wire::ByEdges(IEnumerable<Edge^>^ edges)
	{
		return gcnew Wire(edges);
	}

	Wire^ Wire::ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve ^ polycurve)
	{
		return gcnew Wire(polycurve);
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

		Object^ pDynamoReturnValue = nullptr;
		try
		{
			pDynamoReturnValue = Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurves, 0.001);

			for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
			{
				delete pDynamoCurve;
			}
		}
		catch (std::exception&)
		{
			pDynamoReturnValue = pDynamoCurves;
		}
		catch (Exception^)
		{
			pDynamoReturnValue = pDynamoCurves;
		}
		catch (...)
		{
			throw gcnew Exception("An unknown Wire::Geometry exception is encountered.");
		}

		return pDynamoReturnValue;
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

	Wire::Wire(const TopologicCore::Wire::Ptr& kpCoreWire)
		: Topology()
		, m_pCoreWire(new TopologicCore::Wire::Ptr(kpCoreWire))
	{

	}

	Wire::Wire(IEnumerable<Edge^>^ pEdges)
		: Topology()
		, m_pCoreWire(nullptr)
	{
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		for each(Edge^ pEdge in pEdges)
		{
			coreEdges.push_back(TopologicCore::Topology::Downcast<TopologicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		m_pCoreWire = new TopologicCore::Wire::Ptr(TopologicCore::Wire::ByEdges(coreEdges));
	}

	Wire::Wire(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve)
		: Topology()
		, m_pCoreWire(nullptr)
	{
		array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurves = pDynamoPolycurve->Curves();

		std::list<TopologicCore::Edge::Ptr> coreEdges;
		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
		{
			Edge^ pEdge = gcnew Edge(pDynamoCurve);
			coreEdges.push_back(TopologicCore::Topology::Downcast<TopologicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		m_pCoreWire = new TopologicCore::Wire::Ptr(TopologicCore::Wire::ByEdges(coreEdges));

		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
		{
			delete pDynamoCurve;
		}
	}

	Wire::~Wire()
	{
		//delete m_pCoreWire;
	}
}