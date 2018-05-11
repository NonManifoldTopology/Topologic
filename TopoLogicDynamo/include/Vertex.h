#pragma once

#include <Topology.h>

#include <TopologicCore/include/Vertex.h>

namespace Topologic
{
	ref class Edge;

	public ref class Vertex : Topology
	{
	public:
		/// <summary>
		/// Creates a vertex by a point.
		/// </summary>
		/// <param name="point">A point.</param>
		/// <returns name="Vertex">
		/// A vertex.
		/// </returns>
		static Vertex^ ByPoint(Autodesk::DesignScript::Geometry::Point^ point);

		/// <summary>
		/// Gets the list of edges incident to this vertex.
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="Edge[]">The edges incident to this vertex</returns>
		List<Edge^>^ Edges(Topology^ parentTopology);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
			virtual Object^ get() override;	
		}

	public protected:
		Vertex(const std::shared_ptr<TopologicCore::Vertex>& kpCoreVertex);
		Vertex(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint);

		Autodesk::DesignScript::Geometry::Point^ Point();

		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Vertex();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Vertex>* m_pCoreVertex;
	};
}