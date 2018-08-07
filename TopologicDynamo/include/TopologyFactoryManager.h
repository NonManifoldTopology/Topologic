#pragma once

#include <Topology.h>

//#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic
{
	ref class TopologyFactory;

	ref class TopologyFactoryManager
	{
	public protected:
		static property TopologyFactoryManager^ Instance {
			TopologyFactoryManager^ get() { return %m_instance; }
		}

		void Add(const TopologicCore::Topology::Ptr& content, TopologyFactory^ value);

		void Add(String^ key, TopologyFactory^ value);

		TopologyFactory^ Find(String^ key);

		static TopologyFactory^ GetDefaultFactory(const TopologicCore::Topology::Ptr& content);

	private:
		TopologyFactoryManager() {}
		TopologyFactoryManager(const TopologyFactoryManager%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }
		static TopologyFactoryManager m_instance;
		Dictionary<String^, TopologyFactory^>^ m_TopologyFactoryDict = gcnew Dictionary<String^, TopologyFactory^>();
	};
}