#pragma once

#include "Topology.h"

using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

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
			[IsVisibleInDynamoLibrary(false)]
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) abstract;

		protected:
			TopologyFactory() {}
		};
	}
}