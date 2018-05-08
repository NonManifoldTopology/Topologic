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

	List<Edge^>^ Vertex::Edges(Topology^ parentTopology)
	{
		std::list<std::shared_ptr<TopoLogicCore::Edge>> coreEdges;
		std::shared_ptr<TopoLogicCore::Topology> pCoreParentTopology = TopoLogicCore::Topology::Downcast<TopoLogicCore::Topology>(parentTopology->GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Vertex> pCoreVertex = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Vertex>(GetCoreTopologicalQuery());
		pCoreVertex->Edges(pCoreParentTopology, coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Edge>>::iterator coreEdgeIterator = coreEdges.begin();
			coreEdgeIterator != coreEdges.end();
			coreEdgeIterator++)
		{
			const std::shared_ptr<TopoLogicCore::Edge>& kpCoreEdge = *coreEdgeIterator;
			Edge^ pEdge = gcnew Edge(kpCoreEdge);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	std::shared_ptr<TopoLogicCore::TopologicalQuery> Vertex::GetCoreTopologicalQuery()
	{
		assert(m_pCoreVertex != nullptr && "Vertex::m_pCoreVertex is null.");
		if (m_pCoreVertex == nullptr)
		{
			throw gcnew Exception("Vertex::m_pCoreVertex is null.");
		}

		return *m_pCoreVertex;
	}

	Vertex::Vertex(const std::shared_ptr<TopoLogicCore::Vertex>& kpCoreVertex)
		: Topology()
		, m_pCoreVertex(new std::shared_ptr<TopoLogicCore::Vertex>(kpCoreVertex))
	{

	}

	Vertex::Vertex(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint)
		: Vertex(TopoLogicCore::Vertex::ByPoint(new Geom_CartesianPoint(gp_Pnt(pDynamoPoint->X, pDynamoPoint->Y, pDynamoPoint->Z))))
	{

	}

	Autodesk::DesignScript::Geometry::Point^ Vertex::Point()
	{
		gp_Pnt occtPoint = BRep_Tool::Pnt(
			TopoDS::Vertex(
				TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery())->GetOcctShape()
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