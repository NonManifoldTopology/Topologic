// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

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
		IList<double>^ coordinate = Coordinates;
		return Autodesk::DesignScript::Geometry::Point::ByCoordinates(coordinate[0], coordinate[1], coordinate[2]);
	}
#endif

	Vertex ^ Vertex::ByCoordinates(double x, double y, double z)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Vertex::ByCoordinates(x, y, z);
		return gcnew Vertex(pCoreVertex);
	}

	IList<Edge^>^ Vertex::Edges::get()
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

	IList<double>^ Vertex::Coordinates::get()
	{
		List<double>^ coordinate = gcnew List<double>();
		TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(GetCoreTopologicalQuery());
		try {
			std::tuple<double, double, double> coreCoordinate = pCoreVertex->Coordinates();
			coordinate->Add(std::get<0>(coreCoordinate));
			coordinate->Add(std::get<1>(coreCoordinate));
			coordinate->Add(std::get<2>(coreCoordinate));
			return coordinate;
		}
		catch (const std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}
}