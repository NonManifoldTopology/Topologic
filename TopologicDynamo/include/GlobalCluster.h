#pragma once

#ifdef _DEBUG
#include "Topology.h"
#include <TopologicCore/include/GlobalCluster.h>

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic
{
	public ref class GlobalCluster
	{
	public:
		/// <summary>
		/// Returns all sub-topologies under this topology.
		/// </summary>
		/// <returns name="Topology[]">The sub-topologies of the input topology</returns>
		static List<Topology^>^ SubTopologies(Topology^ topology);

	protected:
		GlobalCluster() {}
	};
}
#endif
