#include "TopologyFactoryDictionary.h"

namespace Topologic
{
	void TopologyFactoryDictionary::Add(const TopologicCore::Topology::Ptr& content, TopologyFactory^ value)
	{
		Add(content->GetType(), value);
	}

	void TopologyFactoryDictionary::Add(int key, TopologyFactory^ value)
	{
		try {
			m_TopologyFactoryDict->Add(key, value);
		}
		catch (Exception^)
		{

		}
	}

	TopologyFactory^ TopologyFactoryDictionary::Find(int key)
	{
		if (m_TopologyFactoryDict->ContainsKey(key))
		{
			return m_TopologyFactoryDict[key];
		}
		else
		{
			throw gcnew Exception("Not found");
		}
	}
}