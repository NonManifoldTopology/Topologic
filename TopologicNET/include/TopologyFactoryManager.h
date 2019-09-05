#pragma once

#include "Topology.h"

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic
{
	namespace Factories
	{
		ref class TopologyFactory;

		/// <summary>
		/// A TopologyFactoryManager stores the subclasses of TopologyFactory, identified by their IDs. This class is used to identify the Topology to be created from an OCCT shape. The ID is in the GUID format, represented as a String.  A user-created TopologyFactory must be registered with a unique ID to this class.
		/// </summary>

		ref class TopologyFactoryManager
		{
		public protected:
			/// <summary>
			/// Returns the instance of the TopologyFactoryManager.
			/// </summary>
			/// <returns name="Topology">The TopologyFactoryManager's instance</returns>
			static property TopologyFactoryManager^ Instance {
				TopologyFactoryManager^ get() { return %m_instance; }
			}

			/// <summary>
			/// Adds a TopologyFactory with the key given by the Topology's ID.
			/// </summary>
			/// <param name="topology">The Topology</param>
			/// <param name="value">The TopologyFactory</param>
			void Add(const TopologicCore::Topology::Ptr& topology, TopologyFactory^ value);

			/// <summary>
			/// Adds a TopologyFactory associated to a key.
			/// </summary>
			/// <param name="key">A key</param>
			/// <param name="value">The TopologyFactory</param>
			void Add(String^ key, TopologyFactory^ value);

			/// <summary>
			/// Returns the correct TopologyFactory given a key.
			/// </summary>
			/// <param name="key">A key</param>
			/// <returns name="TopologyFactory">The TopologyFactory</returns>
			TopologyFactory^ Find(String^ key);

			/// <summary>
			/// Returns the default TopologyFactory for a TopologicCore Topology.
			/// </summary>
			/// <param name="topology">A TopologicCore layer Topology</param>
			/// <returns name="TopologyFactory">The default TopologyFactory</returns>
			static TopologyFactory^ GetDefaultFactory(const TopologicCore::Topology::Ptr& topology);

		private:
			TopologyFactoryManager() {}
			TopologyFactoryManager(const TopologyFactoryManager%) { throw gcnew System::InvalidOperationException("Singleton cannot be copy-constructed"); }
			static TopologyFactoryManager m_instance;
			Dictionary<String^, TopologyFactory^>^ m_TopologyFactoryDict = gcnew Dictionary<String^, TopologyFactory^>();
		};
	}
}