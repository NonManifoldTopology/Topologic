#pragma once

#include "Topology.h"

using namespace System::Collections::Generic;
#ifdef TOPOLOGIC_DYNAMO
using namespace Autodesk::DesignScript::Runtime;
#endif

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// A public abstract factory class to create a Topology. This class should be inherited when new topological classes are implemented.
		/// </summary>
		public ref class TopologyFactory abstract
		{
		public:
			/// <summary>
			/// Creates a Topology from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Topology</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) abstract;

		protected:
			TopologyFactory() {}
		};
	}
}