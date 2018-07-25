#include "ContentDictionary.h"

namespace Topologic
{
	void ContentDictionary::Add(const TopologicCore::Topology::Ptr& content, ContentFactory^ value)
	{
		Add(gcnew String(content->GetGUID().c_str()), value);
	}

	void ContentDictionary::Add(String^ key, ContentFactory^ value)
	{
		try {
			m_contentFactoryDict->Add(key, value);
		}
		catch (Exception^)
		{

		}
	}

	ContentFactory^ ContentDictionary::Find(String^ key)
	{
		if (m_contentFactoryDict->ContainsKey(key))
		{
			return m_contentFactoryDict[key];
		}
		else
		{
			throw gcnew Exception("Not found");
		}
	}
}