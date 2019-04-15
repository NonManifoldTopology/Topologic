#include <Vertex.h>
#include <Edge.h>
#include <VertexFactory.h>

#include <assert.h>
#include <tuple>

namespace Topologic
{
#ifdef TOPOLOGIC_DYNAMO
	Vertex^ Vertex::ByPoint(Autodesk::DesignScript::Geometry::Point^ point)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Vertex::ByCoordinates(point->X, point->Y, point->Z);

		return gcnew Vertex(pCoreVertex);
	}

	Autodesk::DesignScript::Geometry::Point^ Vertex::Point()
	{
		List<double>^ coordinate = Coordinates;
		return Autodesk::DesignScript::Geometry::Point::ByCoordinates(coordinate[0], coordinate[1], coordinate[2]);
	}
#endif

	Vertex ^ Vertex::ByCoordinates(double x, double y, double z)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Vertex::ByCoordinates(x, y, z);
		return gcnew Vertex(pCoreVertex);
	}

	List<Edge^>^ Vertex::Edges::get()
	{
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());
		try {
			pCoreVertex->Edges(coreEdges);
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

	int Vertex::Type()
	{
		return TopologicCore::Vertex::Type();
	}

	Vertex::Vertex()
		: Topology()
		, m_pCoreVertex(nullptr)
	{

	}

	Vertex::Vertex(const TopologicCore::Vertex::Ptr& kpCoreVertex)
		: Topology()
		, m_pCoreVertex(kpCoreVertex != nullptr? new TopologicCore::Vertex::Ptr(kpCoreVertex) : throw gcnew Exception("A null vertex was created."))
	{

	}

	Vertex::~Vertex()
	{
		delete m_pCoreVertex;
	}

	Object^ Vertex::BasicGeometry::get()
	{
#ifdef TOPOLOGIC_DYNAMO
		return Point();
#else
		return nullptr;
#endif
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

	List<double>^ Vertex::Coordinates::get()
	{
		List<double>^ coordinate = gcnew List<double>();
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());
		std::tuple<double, double, double> coreCoordinate = pCoreVertex->Coordinates();
		coordinate->Add(std::get<0>(coreCoordinate));
		coordinate->Add(std::get<1>(coreCoordinate));
		coordinate->Add(std::get<2>(coreCoordinate));
		return coordinate;
	}
}