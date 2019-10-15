// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

#pragma once

#include "TopologyFactory.h"

namespace Topologic
{
	/// <summary>
	/// Factories include the following private classes: ApertureFactory, CellComplexFactory, CellFactory, ClusterFactory, EdgeFactory, FaceFactory, ShellFactory, TopologyFactory, VertexFactory, WireFactory and TopologyFactoryManager.
	/// </summary>
	namespace Factories
	{
		/// <summary>
		/// (private) A factory class to create an Aperture.
		/// </summary>
		ref class ApertureFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates an Aperture from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Aperture</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}