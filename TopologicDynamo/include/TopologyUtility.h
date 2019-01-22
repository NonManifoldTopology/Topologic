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
			/// <param name="x"></param>
			/// <param name="y"></param>
			/// <param name="z"></param>
			/// <returns></returns>
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			///
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="pointX"></param>
			/// <param name="pointY"></param>
			/// <param name="pointZ"></param>
			/// <param name="directionX"></param>
			/// <param name="directionY"></param>
			/// <param name="directionZ"></param>
			/// <param name="angleInRadian"></param>
			/// <returns></returns>
			static Topology^ Rotate(Topology^ topology, 
				double pointX, double pointY, double pointZ,
				double directionX, double directionY, double directionZ,
				double angleInRadian);

			/// <summary>
			///
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="vertex"></param>
			/// <param name="scaleFactor"></param>
			/// <returns></returns>
			static Topology^ Scale(Topology^ topology, Vertex^ vertex, double scaleFactor);

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