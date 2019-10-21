// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

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