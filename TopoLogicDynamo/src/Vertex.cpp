#include "Vertex.h"
#include <Edge.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS.hxx>

#include <assert.h>

namespace TopoLogic
{
	Vertex^ Vertex::ByPoint(Autodesk::DesignScript::Geometry::Point^ point)
	{
		return gcnew Vertex(point);
	}

	List<Edge^>^ Vertex::Edges()
	{
		std::list<TopoLogicCore::Edge*> coreEdges;
		TopoLogicCore::Vertex* pCoreVertex = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Vertex>(GetCoreTopologicalQuery());
		pCoreVertex->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		List<Object^>^ pDynamoCurves = gcnew List<Object^>();
		for (std::list<TopoLogicCore::Edge*>::iterator coreEdgeIterator = coreEdges.begin();
			coreEdgeIterator != coreEdges.end();
			coreEdgeIterator++)
		{
			TopoLogicCore::Edge* pCoreEdge = *coreEdgeIterator;
			Edge^ pEdge = gcnew Edge(pCoreEdge);
			pEdges->Add(pEdge);
			pDynamoCurves->Add(pEdge->Geometry);
		}

		return pEdges;
	}

	TopoLogicCore::TopologicalQuery* Vertex::GetCoreTopologicalQuery()
	{
		assert(m_pCoreVertex != nullptr && "Vertex::m_pCoreVertex is null.");
		if (m_pCoreVertex == nullptr)
		{
			throw gcnew Exception("Vertex::m_pCoreVertex is null.");
		}

		return m_pCoreVertex;
	}

	Vertex::Vertex(TopoLogicCore::Vertex * const kpCoreVertex)
		: Topology()
		, m_pCoreVertex(kpCoreVertex)
	{

	}

	Vertex::Vertex(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint)
		: Topology()
		, m_pCoreVertex(TopoLogicCore::Vertex::ByPoint(new Geom_CartesianPoint(gp_Pnt(pDynamoPoint->X, pDynamoPoint->Y, pDynamoPoint->Z))))
	{

	}

	Autodesk::DesignScript::Geometry::Point^ Vertex::Point()
	{
		gp_Pnt occtPoint = BRep_Tool::Pnt(
			TopoDS::Vertex(
				*TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery())->GetOcctShape()
			)
		);
		return Autodesk::DesignScript::Geometry::Point::ByCoordinates(occtPoint.X(), occtPoint.Y(), occtPoint.Z());
	}

	Vertex::~Vertex()
	{
		delete m_pCoreVertex;
	}

	Object^ Vertex::Geometry::get()
	{
		return Point();
	}
}