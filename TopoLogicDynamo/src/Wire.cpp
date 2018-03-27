#include "Wire.h"
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>

#include <assert.h>

namespace TopoLogic
{
	List<Edge^>^ Wire::Edges()
	{
		std::shared_ptr<TopoLogicCore::Wire> pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Edge>> pCoreEdgeList;
		pCoreWire->Edges(pCoreEdgeList);
		List<Edge^>^ pEdges = gcnew List<Edge^>();

		for (std::list<std::shared_ptr<TopoLogicCore::Edge>>::iterator kCoreEdgeIterator = pCoreEdgeList.begin();
			kCoreEdgeIterator != pCoreEdgeList.end();
			kCoreEdgeIterator++)
		{
			std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = *kCoreEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Face^>^ Wire::Faces()
	{
		std::shared_ptr<TopoLogicCore::Wire> pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Face>> pCoreFaceList;
		pCoreWire->Faces(pCoreFaceList);
		List<Face^>^ pFaces = gcnew List<Face^>();

		for (std::list<std::shared_ptr<TopoLogicCore::Face>>::iterator kCoreFaceIterator = pCoreFaceList.begin();
			kCoreFaceIterator != pCoreFaceList.end();
			kCoreFaceIterator++)
		{
			const std::shared_ptr<TopoLogicCore::Face>& kpCoreFace = *kCoreFaceIterator;
			Face^ pFace = gcnew Face(kpCoreFace);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	bool Wire::IsClosed()
	{
		return TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery())->IsClosed();
	}

	List<Vertex^>^ Wire::Vertices()
	{
		std::shared_ptr<TopoLogicCore::Wire> pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Vertex>> pCoreVertexList;
		pCoreWire->Vertices(pCoreVertexList);
		List<Vertex^>^ pVertices = gcnew List<Vertex^>();

		for (std::list<std::shared_ptr<TopoLogicCore::Vertex>>::iterator kCoreVertexIterator = pCoreVertexList.begin();
			kCoreVertexIterator != pCoreVertexList.end();
			kCoreVertexIterator++)
		{
			const std::shared_ptr<TopoLogicCore::Vertex>& pCoreVertex = *kCoreVertexIterator;
			Vertex^ pVertex = gcnew Vertex(pCoreVertex);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	Wire^ Wire::ByEdges(List<Edge^>^ edges)
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

		try
		{
			return Autodesk::DesignScript::Geometry::PolyCurve::ByJoinedCurves(pDynamoCurves);
		}
		catch (std::exception&)
		{
			return pDynamoCurves;
		}
		catch (Exception^)
		{
			return pDynamoCurves;
		}
		catch (...)
		{
			throw gcnew Exception("An unknown Wire::Geometry exception is encountered.");
		}
	}

	std::shared_ptr<TopoLogicCore::TopologicalQuery> Wire::GetCoreTopologicalQuery()
	{
		assert(m_pCoreWire != nullptr && "Wire::m_pCoreWire is null.");
		if (m_pCoreWire == nullptr)
		{
			throw gcnew Exception("Wire::m_pCoreWire is null.");
		}

		return *m_pCoreWire;
	}

	Wire::Wire(const std::shared_ptr<TopoLogicCore::Wire>& kpCoreWire)
		: Topology()
		, m_pCoreWire(new std::shared_ptr<TopoLogicCore::Wire>(kpCoreWire))
	{

	}

	Wire::Wire(List<Edge^>^ pEdges)
		: Topology()
		, m_pCoreWire(nullptr)
	{
		std::list<std::shared_ptr<TopoLogicCore::Edge>> coreEdges;
		for each(Edge^ pEdge in pEdges)
		{
			coreEdges.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		m_pCoreWire = new std::shared_ptr<TopoLogicCore::Wire>(TopoLogicCore::Wire::ByEdges(coreEdges));
	}

	Wire::Wire(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve)
		: Topology()
		, m_pCoreWire(nullptr)
	{
		array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurves = pDynamoPolycurve->Curves();

		std::list<std::shared_ptr<TopoLogicCore::Edge>> coreEdges;
		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
		{
			Edge^ pEdge = gcnew Edge(pDynamoCurve);
			coreEdges.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		m_pCoreWire = new std::shared_ptr<TopoLogicCore::Wire>(TopoLogicCore::Wire::ByEdges(coreEdges));
	}

	Wire::~Wire()
	{
		delete m_pCoreWire;
	}
}