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
			/// Translates (moves) a Topology a certain distance according to XYZ values.
			/// </summary>
			/// <param name="topology">The Topology to be translated</param>
			/// <param name="x">The x value</param>
			/// <param name="y">The y value</param>
			/// <param name="z">The z value</param>
			/// <returns name="Topology">The Topology in its new location</returns>
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			/// Rotates a Topology given an origin, XYZ vectors and an angle.
			/// </summary>
			/// <param name="topology">The Topology to be rotated</param>
			/// <param name="origin">The origin</param>
			/// <param name="xVector">The X vector</param>
			/// <param name="yVector">The Y vector</param>
			/// <param name="zVector">The Z vector</param>
			/// <param name="degree">The angle in degrees</param>
			/// <returns name="Topology">The Topology after its rotation</returns>
			static Topology^ Rotate(Topology^ topology,
				Vertex^ origin,
				double xVector, double yVector, double zVector,
				double degree);

			/// <summary>
			/// Scales a Topology according to an origin and XYZ factors.
			/// </summary>
			/// <param name="topology">The Topology to be scaled</param>
			/// <param name="origin">The origin</param>
			/// <param name="xFactor">The X factor</param>
			/// <param name="yFactor">The Y factor</param>
			/// <param name="zFactor">The Z factor</param>
			/// <returns name="Topology">The Topology after its scaling</returns>
			static Topology^ Scale(Topology^ topology, Vertex^ origin, double xFactor, double yFactor, double zFactor);

			/// <summary>
			/// Returns the distance between two Topologies.
			/// </summary>
			/// <param name="topology">A topology</param>
			/// <param name="anotherTopology">Another Topology</param>
			/// <returns>The distance between the two Topologies</returns>
			static double Distance(Topology^ topology, Topology^ anotherTopology);

			/// <summary>
			/// Transforms a Topology according to translation and rotation factors.
			/// </summary>
			/// <param name="topology">A Topology</param>
			/// <param name="origin">The origin</param>
			/// <param name="xFactor">The X factor</param>
			/// <param name="yFactor">The Y factor</param>
			/// <param name="zFactor">The Z factor</param>
			/// <returns></returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			static Topology^ Transform(Topology^ topology,
				double translationX, double translationY, double translationZ,
				double rotation11, double rotation12, double rotation13,
				double rotation21, double rotation22, double rotation23,
				double rotation31, double rotation32, double rotation33);

		public protected:
			TopologyUtility() {}

			/*static Topology^ Transform(Topology^ topology, 
				double zAxisX, double zAxisY, double zAxisZ,
				double xAxisX, double xAxisY, double xAxisZ);*/

		};
	}
}