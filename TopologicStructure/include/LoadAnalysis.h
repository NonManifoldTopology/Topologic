#pragma once

#include <Load.h>

using namespace System::Collections::Generic;
using namespace System;
using namespace Autodesk::DesignScript::Geometry;
using namespace Autodesk::DesignScript::Runtime;
using namespace Topologic;

namespace TopologicStructure
{
	public ref class LoadAnalysis
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns name="Load[]"></returns>
		static List<Load^>^ Loads(Topologic::Topology^ topology);

	public protected:
	protected:
	};
}