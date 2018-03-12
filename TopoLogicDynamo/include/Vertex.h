#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Vertex.h>

namespace TopoLogic
{
	ref class Edge;

	public ref class Vertex : Topology
	{
	public:
		/// <summary>
		/// Creates a vertex by a point.
		/// </summary>
		/// <param name="point">A point.</param>
		/// <returns>
		/// "TopoLogic Vertex": the created TopoLogic vertex
		/// </returns>
		static Vertex^ ByPoint(Autodesk::DesignScript::Geometry::Point^ point);

		/// <summary>
		/// Gets the list of edges incident to this vertex.
		/// </summary>
		/// <returns>The edges incident to this vertex</returns>
		List<Edge^>^ Edges();

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	public protected:
		Vertex(TopoLogicCore::Vertex* const kpCoreVertex);
		Vertex(Autodesk::DesignScript::Geometry::Point^ pDynamoPoint);

		Autodesk::DesignScript::Geometry::Point^ Point();

		virtual TopoLogicCore::TopologicalQuery* GetCoreTopologicalQuery() override;

	protected:
		virtual ~Vertex();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Vertex* m_pCoreVertex;
	};
}