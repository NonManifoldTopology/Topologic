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
			[IsVisibleInDynamoLibrary(false)]
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			///
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="origin"></param>
			/// <param name="vector"></param>
			/// <param name="degree"></param>
			/// <returns></returns>
			[IsVisibleInDynamoLibrary(false)]
			static Topology^ Rotate(Topology^ topology,
				Vertex^ origin,
				Autodesk::DesignScript::Geometry::Vector^ vector,
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
			[IsVisibleInDynamoLibrary(false)]
			static Topology^ Scale(Topology^ topology, Vertex^ origin, double xFactor, double yFactor, double zFactor);

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