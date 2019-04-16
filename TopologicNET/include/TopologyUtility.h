#pragma once

#include "Shell.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	
		/// <summary>
		/// TopologyUtility includes geometric methods relevant to any Topology.
		/// </summary>
		public ref class TopologyUtility
		{
		public:

			/// <summary>
			/// Translates (moves) a Topology a certain distance according to x, y, z values.
			/// </summary>
			/// <param name="topology">The topology to be translated</param>
			/// <param name="x">The x value</param>
			/// <param name="y">The y value</param>
			/// <param name="z">The z value</param>
			/// <returns>The Topology in its new location</returns>
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			///
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="origin"></param>
			/// <param name="xVector"></param>
			/// <param name="yVector"></param>
			/// <param name="zVector"></param>
			/// <param name="degree"></param>
			/// <returns></returns>
			static Topology^ Rotate(Topology^ topology,
				Vertex^ origin,
				double xVector, double yVector, double zVector,
				double degree);

			/// <summary>
			///
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="origin"></param>
			/// <param name="xFactor"></param>
			/// <param name="yFactor"></param>
			/// <param name="zFactor"></param>
			/// <returns></returns>
			static Topology^ Scale(Topology^ topology, Vertex^ origin, double xFactor, double yFactor, double zFactor);

			/// <summary>
			/// Returns the distance between two Topologies.
			/// </summary>
			/// <param name="topology">A topology</param>
			/// <param name="anotherTopology">Another Topology</param>
			/// <returns>The distance between the two Topologies</returns>
			static double Distance(Topology^ topology, Topology^ anotherTopology);

			/// <summary>
			/// Returns the center of mass of any Topology.
			/// </summary>
			/// <param name="topology">A Topology</param>
			/// <returns>The center of mass of the given Topology</returns>
			static Vertex^ CenterOfMass(Topology^ topology);

		public protected:
			TopologyUtility() {}

			static Topology^ Transform(Topology^ topology, 
				double zAxisX, double zAxisY, double zAxisZ,
				double xAxisX, double xAxisY, double xAxisZ);

		};
	}
}