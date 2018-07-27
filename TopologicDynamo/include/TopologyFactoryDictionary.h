#pragma once

#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic
{
	ref class TopologyFactory;

	ref class TopologyFactoryDictionary
	{
	public protected:
		static property TopologyFactoryDictionary^ Instance {
			TopologyFactoryDictionary^ get() { return %m_instance; }
		}

		void Add(const TopologicCore::Topology::Ptr& content, TopologyFactory^ value);

		void Add(int key, TopologyFactory^ value);

		TopologyFactory^ Find(int key);

	private:
		TopologyFactoryDictionary() {}
		TopologyFactoryDictionary(const TopologyFactoryDictionary%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }
		static TopologyFactoryDictionary m_instance;
		Dictionary<int, TopologyFactory^>^ m_TopologyFactoryDict = gcnew Dictionary<int, TopologyFactory^>();
	};
}