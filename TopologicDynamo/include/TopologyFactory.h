#pragma once

#include "Topology.h"

//#include <TopologicCore/include/Topology.h>

using namespace System::Collections::Generic;
using namespace Autodesk::DesignScript::Runtime;

namespace Topologic
{
	public ref class TopologyFactory abstract
	{
	public:
		/*[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) abstract;

		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(TopologicCore::Topology const * const kpTopology) abstract;*/

		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) abstract;

	protected:
		TopologyFactory() {}
	};
}