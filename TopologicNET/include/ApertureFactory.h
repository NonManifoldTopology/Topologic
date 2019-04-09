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
		/// (private) A private factory class to create an Aperture.
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