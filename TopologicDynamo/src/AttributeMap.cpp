#include <msclr/marshal_cppstd.h>

#include <AttributeMap.h>

#include <TopologicSupport/include/IntAttribute.h>
#include <TopologicSupport/include/DoubleAttribute.h>

#include <memory>

namespace Topologic {
	namespace Support
	{
		/*AttributeMap^ AttributeMap::ByDictionary(Dictionary<String^, Object^>^ dictionary)
		{
			TopologicSupport::AttributeMap::Ptr pCoreAttributeMap = std::make_shared<TopologicSupport::AttributeMap>();
			List<String^>^ keys = gcnew List<String^>(dictionary->Keys);
			for each(String^ key in keys)
			{
				Object^ value = dictionary[key];
				AddToCoreAttributeMap(pCoreAttributeMap, key, value);
			}
			return gcnew AttributeMap(pCoreAttributeMap);
		}*/

		AttributeMap^ AttributeMap::ByDictionary(List<String^>^ keys, List<Object^>^ values)//Dictionary<String^, Object^>^ dictionary)
		{
			if (keys->Count != values->Count)
			{
				throw gcnew Exception("keys and values must have the same size");
			}
			TopologicSupport::AttributeMap::Ptr pCoreAttributeMap = std::make_shared<TopologicSupport::AttributeMap>();
			for(int i = 0; i < keys->Count; i++)
			{
				AddToCoreAttributeMap(pCoreAttributeMap, keys[i], values[i]);
			}
			return gcnew AttributeMap(pCoreAttributeMap);
		}

		Dictionary<String^, Object^>^ AttributeMap::GetDictionary()
		{
			Dictionary<String^, Object^>^ dictionary = gcnew Dictionary<String^, Object^>();
			for (auto const& kpMapPair : (**m_pCoreAttributeMap))
			{
				dictionary->Add(gcnew String(kpMapPair.first.c_str()), GetCoreValueToManaged(kpMapPair.second));
			}
			return dictionary;
		}

		AttributeMap::AttributeMap(const TopologicSupport::AttributeMap::Ptr& kpCoreAttributeMap)
			: m_pCoreAttributeMap(new TopologicSupport::AttributeMap::Ptr(kpCoreAttributeMap))
		{

		}

		AttributeMap::~AttributeMap()
		{
			//delete m_pCoreAttribute;
		}

		void AttributeMap::AddToCoreAttributeMap(TopologicSupport::AttributeMap::Ptr & rAttributeMap, String ^ key, Object ^ value)
		{
			std::string cppKey = msclr::interop::marshal_as<std::string>(key);
			Type^ type = value->GetType();
			try {
				__int64 i = safe_cast<__int64>(value);
				TopologicSupport::IntAttribute::Ptr intAttribute = std::make_shared<TopologicSupport::IntAttribute>(i);
				rAttributeMap->Add(cppKey, std::dynamic_pointer_cast<TopologicSupport::Attribute>(intAttribute));
			}
			catch (...)
			{

			}
			try {
				double i = safe_cast<double>(value);
				TopologicSupport::DoubleAttribute::Ptr doubleAttribute = std::make_shared<TopologicSupport::DoubleAttribute>(i);
				rAttributeMap->Add(cppKey, std::dynamic_pointer_cast<TopologicSupport::Attribute>(doubleAttribute));
			}
			catch (...)
			{

			}
		}

		Object ^ AttributeMap::GetCoreValueToManaged(const TopologicSupport::Attribute::Ptr& kpAttribute)
		{
			TopologicSupport::IntAttribute::Ptr intAttribute = std::dynamic_pointer_cast<TopologicSupport::IntAttribute>(kpAttribute);
			if (intAttribute != nullptr)
			{
				return intAttribute->Value();
			}

			TopologicSupport::DoubleAttribute::Ptr doubleAttribute = std::dynamic_pointer_cast<TopologicSupport::DoubleAttribute>(kpAttribute);
			if (doubleAttribute != nullptr)
			{
				return doubleAttribute->Value();
			}

			return nullptr;
		}
	}
}