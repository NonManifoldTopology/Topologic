#include "ContentDictionary.h"

namespace Topologic
{
	void ContentDictionary::Add(const TopologicCore::Topology::Ptr& content, ContentFactory^ value)
	{
		Add(content->GetType(), value);
	}

	void ContentDictionary::Add(int key, ContentFactory^ value)
	{
		try {
			m_contentFactoryDict->Add(key, value);
		}
		catch (Exception^)
		{

		}
	}

	ContentFactory^ ContentDictionary::Find(int key)
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