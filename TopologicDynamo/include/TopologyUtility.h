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