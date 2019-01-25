#pragma once

#include "Shell.h"

namespace Topologic {
	/// <summary>
	/// Utilities provide extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
	namespace Utilities {
	
		/// <summary>
		/// TopologyUtility includes geometric methods relevant to any topology.
		/// </summary>
		public ref class TopologyUtility
		{
		public:

			/// <summary>
			/// Translates (moves) a topology a certain distance according to x, y, z values.
			/// </summary>
			/// <param name="topology">The topology to be translated</param>
			/// <param name="x">The x value</param>
			/// <param name="y">The y value</param>
			/// <param name="z">The z value</param>
			/// <returns>The topology in its new location</returns>
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			/// Returns the distance between two topologies.
			/// </summary>
			/// <param name="topology">A topology</param>
			/// <param name="anotherTopology">Another topology</param>
			/// <returns>The distance between the two topologies</returns>
			static double Distance(Topology^ topology, Topology^ anotherTopology);

			/// <summary>
			/// Returns the center of mass of any topology.
			/// </summary>
			/// <param name="topology">A topology</param>
			/// <returns>The center of mass of the given topology</returns>
			static Vertex^ CenterOfMass(Topology^ topology);

		public protected:
			TopologyUtility() {}
		};
	}
}