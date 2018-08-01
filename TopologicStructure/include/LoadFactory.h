#pragma once

#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace Autodesk::DesignScript::Runtime;

namespace TopologicStructure
{
	ref class LoadFactory : Topologic::TopologyFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Topologic::Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;

		static String^ GetGUID() { return gcnew String("52bb61cc-1453-46fd-9bc0-fe7d5e7be0e7"); }
	};
}