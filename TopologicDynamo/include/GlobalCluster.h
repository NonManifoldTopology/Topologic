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
		/// 
		/// </summary>
		/// <param name="refresh"></param>
		/// <returns></returns>
		static List<Topology^>^ SubTopologies(bool refresh);

	protected:
		GlobalCluster() {}
	};
}
#endif
