#include "Vertex.h"
#include <Edge.h>
#include <VertexFactory.h>

#include <assert.h>
#include <tuple>

namespace Topologic
{
	Vertex^ Vertex::ByPoint(Autodesk::DesignScript::Geometry::Point^ point)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Vertex::ByCoordinates(point->X, point->Y, point->Z);

		return gcnew Vertex(pCoreVertex);
	}

	Vertex ^ Vertex::ByCoordinates(double x, double y, double z)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Vertex::ByCoordinates(x, y, z);
		return gcnew Vertex(pCoreVertex);
	}

	List<Edge^>^ Vertex::Edges_(Topology^ hostTopology)
	{
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(hostTopology->GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());
		try {
			pCoreVertex->Edges(pCoreParentTopology, coreEdges);
		}catch(const std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}

		List<Edge^>^ edges = gcnew List<Edge^>();
		for (std::list<TopologicCore::Edge::Ptr>::iterator coreEdgeIterator = coreEdges.begin();
			coreEdgeIterator != coreEdges.end();
			coreEdgeIterator++)
		{
			const TopologicCore::Edge::Ptr& kpCoreEdge = *coreEdgeIterator;
			Edge^ pEdge = gcnew Edge(kpCoreEdge);
			edges->Add(pEdge);
		}

		return edges;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Vertex::GetCoreTopologicalQuery()
	{
		assert(m_pCoreVertex != nullptr && "Vertex::m_pCoreVertex is null.");
		if (m_pCoreVertex == nullptr)
		{
			throw gcnew Exception("Vertex::m_pCoreVertex is null.");
		}

		return *m_pCoreVertex;
	}

	Vertex::Vertex(const TopologicCore::Vertex::Ptr& kpCoreVertex)
		: Topology()
		, m_pCoreVertex(kpCoreVertex != nullptr? new TopologicCore::Vertex::Ptr(kpCoreVertex) : throw gcnew Exception("A null vertex is created."))
	{

	}

	Autodesk::DesignScript::Geometry::Point^ Vertex::Point()
	{
		List<double>^ coordinate = Coordinate;
		return Autodesk::DesignScript::Geometry::Point::ByCoordinates(coordinate[0], coordinate[1], coordinate[2]);
	}

	Vertex::~Vertex()
	{
		//delete m_pCoreVertex;
	}

	Object^ Vertex::Geometry::get()
	{
		return Point();
	}

	double Vertex::X::get()
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());

		return pCoreVertex->X();
	}

	double Vertex::Y::get()
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());

		return pCoreVertex->Y();
	}

	double Vertex::Z::get()
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());

		return pCoreVertex->Z();
	}

	List<double>^ Vertex::Coordinate::get()
	{
		List<double>^ coordinate = gcnew List<double>();
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());
		std::tuple<double, double, double> coreCoordinate = pCoreVertex->Coordinate();
		coordinate->Add(std::get<0>(coreCoordinate));
		coordinate->Add(std::get<1>(coreCoordinate));
		coordinate->Add(std::get<2>(coreCoordinate));
		return coordinate;
	}
}