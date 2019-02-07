#pragma once

#include "Edge.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	
		/// <summary>
		/// EdgeUtility includes geometric methods relevant to an Edge.
		/// </summary>
		public ref class EdgeUtility
		{
		public:
			/// <summary>
			/// Create an Edge (of any type) by a set of Vertices.
			/// </summary>
			/// <param name="vertices">A set of Vertices</param>
			/// <returns>The created Edge</returns>
			static Edge^ ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices);

			/// <summary>
			/// Create an Edge by a circle.
			/// </summary>
			/// <param name="centerPoint">The center point of the circle</param>
			/// <param name="radius">The radius of the circle</param>
			/// <param name="xAxisX">The starting X value of the circle on the X axis</param>
			/// <param name="xAxisY">The starting X value of the circle on the Y axis</param>
			/// <param name="xAxisZ">The starting X value of the circle on the Z axis</param>
			/// <param name="normalX">The X component of the circle's normal</param>
			/// <param name="normalY">The Y component of the circle's normal</param>
			/// <param name="normalZ">The Z component of the circle's normal</param>
			/// <returns>The created Edge</returns>
			static Edge^ ByCircle(
				Vertex^ centerPoint, double radius,
				double xAxisX, double xAxisY, double xAxisZ,
				double normalX, double normalY, double normalZ);

			/// <summary>
			/// Returns the parameter at a given Vertex on the Edge.
			/// </summary>
			/// <param name="edge">The Edge</param>
			/// <param name="vertex">The Vertex at which the parameter is sought</param>
			/// <returns>The parameter at a given Vertex on the Edge</returns>
			static double ParameterAtVertex(Edge^ edge, Vertex^ vertex);

			/// <summary>
			/// Returns the Vertex at a given parameter of the Edge.
			/// </summary>
			/// <param name="edge">The Edge for which the parameter is given</param>
			/// <param name="parameter">The parameter of the Edge</param>
			/// <returns>The Vertex at a given parameter of the Edge</returns>
			static Vertex^ VertexAtParameter(Edge^ edge, double parameter);

		public protected:
			EdgeUtility() {}
		};
	}
}