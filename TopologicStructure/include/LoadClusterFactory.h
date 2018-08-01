#pragma once

#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicStructure
{
	ref class LoadClusterFactory : Topologic::TopologyFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Topologic::Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;

		static String^ GetGUID() { return gcnew String("7347bff1-b02b-4029-a003-c2f7882e2de1"); }
	};
}