#pragma once

#include <TopologicSupport/include/Attribute.h>
#include <TopologicSupport/include/AttributeMap.h>

#include <memory>

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic {
	namespace Support
	{
		public ref class AttributeMap
		{
		public:
			//static AttributeMap^ ByDictionary(Dictionary<String^, Object^>^ dictionary);
			static AttributeMap^ ByDictionary(List<String^>^ keys, List<Object^>^ values);

			Dictionary<String^, Object^>^ GetDictionary();

		public protected:
			AttributeMap(const TopologicSupport::AttributeMap::Ptr& kpCoreAttribute);

		protected:
			~AttributeMap();

			static void AddToCoreAttributeMap(TopologicSupport::AttributeMap::Ptr& rAttributeMap, String^ key, Object^ value);

			static Object^ GetCoreValueToManaged(const TopologicSupport::Attribute::Ptr& kpAttribute);

			TopologicSupport::AttributeMap::Ptr* m_pCoreAttributeMap;
		};
	}
}