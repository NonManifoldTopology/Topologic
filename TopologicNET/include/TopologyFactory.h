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