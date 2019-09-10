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
			/// Creates an Edge (of any type) by a set of Vertices.
			/// </summary>
			/// <param name="vertices">A set of Vertices</param>
			/// <returns>The created Edge</returns>
			static Edge^ ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices);

			/// <summary>
			/// Creates an Edge by a circle.
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
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Edge^ ByCircle(
				Vertex^ centerPoint, double radius,
				double xAxisX, double xAxisY, double xAxisZ,
				double normalX, double normalY, double normalZ);

			/// <summary>
			/// Creates an Edge by an ellipse.
			/// </summary>
			/// <param name="centerPoint">The center point of the ellipse</param>
			/// <param name="majorRadius">The major radius of the ellipse</param>
			/// <param name="minorRadius">The minor radius of the ellipse</param>
			/// <param name="xAxisX">The starting X value of the ellipse on the X axis</param>
			/// <param name="xAxisY">The starting X value of the ellipse on the Y axis</param>
			/// <param name="xAxisZ">The starting X value of the ellipse on the Z axis</param>
			/// <param name="normalX">The X component of the ellipse's normal</param>
			/// <param name="normalY">The Y component of the ellipse's normal</param>
			/// <param name="normalZ">The Z component of the ellipse's normal</param>
			/// <returns>The created Edge</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Edge^ ByEllipse(
				Vertex^ centerPoint, double majorRadius, double minorRadius,
				double xAxisX, double xAxisY, double xAxisZ,
				double normalX, double normalY, double normalZ);

			/// <summary>
			/// Creates an Edge by a NURBS curve.
			/// </summary>
			/// <param name="controlPoints">A list of control points</param>
			/// <param name="knots">A list of knots</param>
			/// <param name="weights">A list of weights</param>
			/// <param name="degree">The degree</param>
			/// <param name="isPeriodic">True if it is periodic, otherwise false</param>
			/// <param name="isRational">True if it is rational, otherwise false</param>
			/// <returns>The created Edge</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif	
			static Edge^ ByNurbsCurve(
				List<Vertex^>^ controlPoints, 
				List<double>^ knots,
				List<double>^ weights,
				int degree,
				bool isPeriodic,
				bool isRational);

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
			/// <param name="u">The U parametric coordinate of the Vertex on the Edge</param>
			/// <returns>The Vertex at a given parameter of the Edge</returns>
			static Vertex^ VertexAtParameter(Edge^ edge, double u);

			/// <summary>
			/// Returns a list of Wires that are the adjacent to the input Edge.
			/// </summary>
			/// <param name="edge">An Edge</param>
			/// <param name="parentTopology">A parent Topology</param>
			/// <returns name="Wire[]">A list of Wires adjacent to the input edge</returns>
			static List<Wire^>^ AdjacentWires(Edge^ edge, Topology^ parentTopology);

		public protected:
			EdgeUtility() {}
		};
	}
}