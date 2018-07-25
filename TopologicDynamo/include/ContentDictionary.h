#pragma once

#include <TopologicCore/include/Topology.h>

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic
{
	ref class ContentFactory;

	ref class ContentDictionary
	{
	private:
		ContentDictionary() {}
		ContentDictionary(const ContentDictionary%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }
		static ContentDictionary m_instance;
		Dictionary<String^, ContentFactory^>^ m_contentFactoryDict = gcnew Dictionary<String^, ContentFactory^>();

	public protected:
		static property ContentDictionary^ Instance {
			ContentDictionary^ get() { return %m_instance; }
		}

		void Add(const TopologicCore::Topology::Ptr& content, ContentFactory^ value);

		void Add(String^ key, ContentFactory^ value);

		ContentFactory^ Find(String^ key);
	};
}