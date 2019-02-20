#include <msclr/marshal_cppstd.h>
#include "StringAttributeFactory.h"

#include <TopologicUtilities/include/StringAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ StringAttributeFactory::Create(String ^ key, Object ^ value)
		{
			String^ strValue = safe_cast<String^>(value);
			std::string cppStrValue = msclr::interop::marshal_as<std::string>(strValue);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicUtilities::Attribute>(std::make_shared<TopologicUtilities::StringAttribute>(cppStrValue)));
		}

		bool StringAttributeFactory::CheckType(Type ^ type)
		{
			System::Type^ stringHandleType = String::typeid;

			return type == stringHandleType;
		}

		bool StringAttributeFactory::CheckType(const std::shared_ptr<TopologicUtilities::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicUtilities::StringAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ StringAttributeFactory::CreateValue(const std::shared_ptr<TopologicUtilities::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			std::string* pStringValue = static_cast<std::string*>(pValue);
			return gcnew String(pStringValue->c_str());
		}
	}
}