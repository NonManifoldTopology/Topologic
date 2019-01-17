#pragma once

#include "Shell.h"

namespace Topologic {
	namespace Utilities {
	/// <summary>
	/// Utility provides extended functionality including further topologic and geometric methods and properties. These include the Bitwise, CellUtility, EdgeUtility, FaceUtility, ShellUtility and TopologyUtility classes.
	/// </summary>
		/// <summary>
		/// TopologyUtility includes geometric methods relevant to any topology.
		/// </summary>
		public ref class TopologyUtility
		{
		public:

			/// <summary>
			/// Translate (move) a topology a certain distance according to x, y, z values.
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="y"></param>
			/// <param name="z"></param>
			/// <returns></returns>
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			/// Returns the distance between two topologies.
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="anotherTopology"></param>
			/// <returns></returns>
			static double Distance(Topology^ topology, Topology^ anotherTopology);

			/// <summary>
			/// Returns the center of mass of any topology.
			/// </summary>
			/// <param name="topology"></param>
			/// <returns></returns>
			static Vertex^ CenterOfMass(Topology^ topology);

		public protected:
			TopologyUtility() {}
		};
	}
}