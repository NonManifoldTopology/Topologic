#include "TopologyFactoryDictionary.h"

namespace Topologic
{
	void TopologyFactoryDictionary::Add(const TopologicCore::Topology::Ptr& content, TopologyFactory^ value)
	{
		Add(gcnew String(content->GetClassGUID().c_str()), value);
	}

	void TopologyFactoryDictionary::Add(String^ key, TopologyFactory^ value)
	{
		try {
			m_TopologyFactoryDict->Add(key, value);
		}
		catch (Exception^)
		{

		}
	}

	TopologyFactory^ TopologyFactoryDictionary::Find(String^ key)
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