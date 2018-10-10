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
		/// Create a vertex by XYZ coordinates.
		/// </summary>
		/// <param name="x">The x coordinate.</param>
		/// <param name="y">The y coordinate.</param>
		/// <param name="z">The z coordinate.</param>
		/// <returns name="Vertex">
		/// A vertex.
		/// </returns>
		static Vertex^ ByCoordinates(double x, double y, double z);

		/// <summary>
		/// Get the list of edges incident to the vertex. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Edge[]">The edges incident to this vertex</returns>
		List<Edge^>^ Edges_(Topology^ hostTopology);

		property double X
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual double get();
		}

		property double Y
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual double get();
		}

		property double Z
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual double get();
		}

		property List<double>^ Coordinate
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
			virtual List<double>^ get();
		}

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns name="Geometry"></returns>
			virtual Object^ get() override;	
		}

	public protected:
		Vertex(Vertex^ anotherVertex);
		Vertex(Autodesk::DesignScript::Geometry::Point^ dynamoPoint);
		Vertex(const std::shared_ptr<TopologicCore::Vertex>& kpCoreVertex);

		/// <summary>
		/// Create a vertex by a point. NOTE: This node will be replaced by a single Topology.ByGeometry() node.
		/// </summary>
		/// <param name="point">A point.</param>
		/// <returns name="Vertex">
		/// A vertex.
		/// </returns>
		static Vertex^ ByPoint_(Autodesk::DesignScript::Geometry::Point^ point);

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