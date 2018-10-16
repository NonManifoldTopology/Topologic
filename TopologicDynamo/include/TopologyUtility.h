#pragma once

#include "Shell.h"

namespace Topologic {
	namespace Support {
		/// <summary>
		/// 
		/// </summary>
		public ref class TopologyUtility
		{
		public:

			/// <summary>
			/// 
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="y"></param>
			/// <param name="z"></param>
			/// <returns></returns>
			static Topology^ Translate(Topology^ topology, double x, double y, double z);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="topology"></param>
			/// <param name="anotherTopology"></param>
			/// <returns></returns>
			static double Distance(Topology^ topology, Topology^ anotherTopology);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="topology"></param>
			/// <returns></returns>
			static Vertex^ CenterOfMass(Topology^ topology);

		public protected:
			TopologyUtility() {}
		};
	}
}