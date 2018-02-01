#include <Wire.h>
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>

#include <assert.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Wire::Edges(Wire^ topoLogicWire)
	{
		List<Edge^>^ pEdges = topoLogicWire->Edges();
		List<Object^>^ pDynamoCurves = gcnew List<Object^>();

		for each(Edge^ pEdge in pEdges)
		{
			pDynamoCurves->Add(pEdge->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edges", pEdges);
		pDictionary->Add("Curves", pDynamoCurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Wire::Faces(Wire ^ topoLogicWire)
	{
		TopoLogicCore::Wire* pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(topoLogicWire->GetCoreTopology());
		std::list<TopoLogicCore::Face*> pCoreFaceList;
		pCoreWire->Faces(pCoreFaceList);
		List<Face^>^ pFaces = gcnew List<Face^>();
		List<Object^>^ pDynamoSurfaces = gcnew List<Object^>();

		for (std::list<TopoLogicCore::Face*>::iterator kCoreFaceIterator = pCoreFaceList.begin();
			kCoreFaceIterator != pCoreFaceList.end();
			kCoreFaceIterator++)
		{
			TopoLogicCore::Face* pCoreFace = *kCoreFaceIterator;
			Face^ pFace = gcnew Face(pCoreFace);
			pFaces->Add(pFace);
			pDynamoSurfaces->Add(pFace->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pFaces);
		pDictionary->Add("Surfaces", pDynamoSurfaces);
		return pDictionary;
	}

	bool Wire::IsClosed()
	{
		return TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopology());
	}

	Dictionary<String^, Object^>^ Wire::Vertices(Wire^ topoLogicWire)
	{
		TopoLogicCore::Wire* pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(topoLogicWire->GetCoreTopology());
		std::list<TopoLogicCore::Vertex*> pCoreVertexList;
		pCoreWire->Vertices(pCoreVertexList);
		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		List<Object^>^ pDynamoPoints = gcnew List<Object^>();

		for (std::list<TopoLogicCore::Vertex*>::iterator kCoreVertexIterator = pCoreVertexList.begin();
			kCoreVertexIterator != pCoreVertexList.end();
			kCoreVertexIterator++)
		{
			TopoLogicCore::Vertex* pCoreVertex = *kCoreVertexIterator;
			Vertex^ pVertex = gcnew Vertex(pCoreVertex);
			pVertices->Add(pVertex);
			pDynamoPoints->Add(pVertex->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertices", pVertices);
		pDictionary->Add("Points", pDynamoPoints);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Wire::ByEdges(List<Edge^>^ edges)
	{
		Wire^ pWire = gcnew Wire(edges);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Wire", pWire);
		pDictionary->Add("PolyCurve", pWire->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Wire::ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve ^ polycurve)
	{
		Wire^ pWire = gcnew Wire(polycurve);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Wire", pWire);
		pDictionary->Add("PolyCurve", pWire->Geometry);
		return pDictionary;
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
			throw gcnew Exception(gcnew String(e.what()));
		}
		catch (Exception^ e)
		{
			throw gcnew Exception(e->Message);
		}
		catch (...)
		{
			throw gcnew Exception("An unknown Wire::Geometry exception is encountered.");
		}
	}

	TopoLogicCore::Topology* Wire::GetCoreTopology()
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
			coreEdges.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(pEdge->GetCoreTopology()));
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
			coreEdges.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(pEdge->GetCoreTopology()));
		}

		m_pCoreWire = TopoLogicCore::Wire::ByEdges(coreEdges);
	}

	List<Edge^>^ Wire::Edges()
	{
		TopoLogicCore::Wire* pCoreWire = TopoLogicCore::Topology::Downcast<TopoLogicCore::Wire>(GetCoreTopology());
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

	Wire::~Wire()
	{
		delete m_pCoreWire;
	}
}