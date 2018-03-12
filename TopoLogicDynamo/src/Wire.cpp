#include "Wire.h"
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>

#include <assert.h>

namespace TopoLogic
{
	List<Edge^>^ Wire::Edges()
	{
		TopoLogicCore::Wire* pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery());
		std::list<TopoLogicCore::Edge*> pCoreEdgeList;
		pCoreWire->Edges(pCoreEdgeList);
		List<Edge^>^ pEdges = gcnew List<Edge^>();

		for (std::list<TopoLogicCore::Edge*>::iterator kCoreEdgeIterator = pCoreEdgeList.begin();
			kCoreEdgeIterator != pCoreEdgeList.end();
			kCoreEdgeIterator++)
		{
			TopoLogicCore::Edge* pCoreEdge = *kCoreEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Face^>^ Wire::Faces()
	{
		TopoLogicCore::Wire* pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery());
		std::list<TopoLogicCore::Face*> pCoreFaceList;
		pCoreWire->Faces(pCoreFaceList);
		List<Face^>^ pFaces = gcnew List<Face^>();

		for (std::list<TopoLogicCore::Face*>::iterator kCoreFaceIterator = pCoreFaceList.begin();
			kCoreFaceIterator != pCoreFaceList.end();
			kCoreFaceIterator++)
		{
			TopoLogicCore::Face* pCoreFace = *kCoreFaceIterator;
			Face^ pFace = gcnew Face(pCoreFace);
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
		TopoLogicCore::Wire* pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopologicalQuery());
		std::list<TopoLogicCore::Vertex*> pCoreVertexList;
		pCoreWire->Vertices(pCoreVertexList);
		List<Vertex^>^ pVertices = gcnew List<Vertex^>();

		for (std::list<TopoLogicCore::Vertex*>::iterator kCoreVertexIterator = pCoreVertexList.begin();
			kCoreVertexIterator != pCoreVertexList.end();
			kCoreVertexIterator++)
		{
			TopoLogicCore::Vertex* pCoreVertex = *kCoreVertexIterator;
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
		catch (std::exception& e)
		{
			return pDynamoCurves;
		}
		catch (Exception^ e)
		{
			return pDynamoCurves;
		}
		catch (...)
		{
			throw gcnew Exception("An unknown Wire::Geometry exception is encountered.");
		}
	}

	TopoLogicCore::TopologicalQuery* Wire::GetCoreTopologicalQuery()
	{
		assert(m_pCoreWire != nullptr && "Wire::m_pCoreWire is null.");
		if (m_pCoreWire == nullptr)
		{
			throw gcnew Exception("Wire::m_pCoreWire is null.");
		}

		return m_pCoreWire;
	}

	Wire::Wire(TopoLogicCore::Wire* const kpCoreWire)
		: Topology()
		, m_pCoreWire(kpCoreWire)
	{

	}

	Wire::Wire(List<Edge^>^ pEdges)
		: Topology()
		, m_pCoreWire(nullptr)
	{
		std::list<TopoLogicCore::Edge*> coreEdges;
		for each(Edge^ pEdge in pEdges)
		{
			coreEdges.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		m_pCoreWire = TopoLogicCore::Wire::ByEdges(coreEdges);
	}

	Wire::Wire(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve)
		: Topology()
		, m_pCoreWire(nullptr)
	{
		array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoCurves = pDynamoPolycurve->Curves();

		std::list<TopoLogicCore::Edge*> coreEdges;
		for each(Autodesk::DesignScript::Geometry::Curve^ pDynamoCurve in pDynamoCurves)
		{
			Edge^ pEdge = gcnew Edge(pDynamoCurve);
			coreEdges.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(pEdge->GetCoreTopologicalQuery()));
		}

		m_pCoreWire = TopoLogicCore::Wire::ByEdges(coreEdges);
	}

	Wire::~Wire()
	{
		delete m_pCoreWire;
	}
}