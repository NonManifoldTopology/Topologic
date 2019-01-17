#pragma once

#include "Edge.h"

namespace Topologic {
	namespace Utilities {
	/// <summary>
	/// Utility provides extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>

		/// <summary>
		/// EdgeUtility includes geometric methods relevant to an Edge.
		/// </summary>
		public ref class EdgeUtility
		{
		public:
			/// <summary>
			/// Create an edge (of any type) by a set of vertices.
			/// </summary>
			/// <param name="vertices">A set of Vertices</param>
			/// <returns>An Edge</returns>
			static Edge^ ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices);

			/// <summary>
			/// Create an edge by a circle.
			/// </summary>
			/// <param name="centerPoint">The center point of the circle</param>
			/// <param name="radius">The radius of the circle</param>
			/// <param name="xAxisX"></param>
			/// <param name="xAxisY"></param>
			/// <param name="xAxisZ"></param>
			/// <param name="normalX">The X component of the circle's normal</param>
			/// <param name="normalY">The Y component of the circle's normal</param>
			/// <param name="normalZ">The Z component of the circle's normal</param>
			/// <returns>An Edge</returns>
			static Edge^ ByCircle(
				Vertex^ centerPoint, double radius,
				double xAxisX, double xAxisY, double xAxisZ,
				double normalX, double normalY, double normalZ);

			/// <summary>
			/// Returns the parameter at a given vertex on the edge.
			/// </summary>
			/// <param name="edge">The edge</param>
			/// <param name="vertex">The vertex at which the parameter is sought</param>
			/// <returns>The parameter at a given vertex on the edge</returns>
			static double ParameterAtVertex(Edge^ edge, Vertex^ vertex);

			/// <summary>
			/// Returns the vertex at a given parameter of the edge.
			/// </summary>
			/// <param name="edge">The edge for which the parameter is given</param>
			/// <param name="parameter">The parameter of the edge</param>
			/// <returns>The vertex at a given parameter of the edge</returns>
			static Vertex^ VertexAtParameter(Edge^ edge, double parameter);

		public protected:
			EdgeUtility() {}
		};
	}
}