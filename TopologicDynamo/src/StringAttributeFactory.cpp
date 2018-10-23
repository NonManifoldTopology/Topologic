#include <msclr/marshal_cppstd.h>
#include "StringAttributeFactory.h"

#include <TopologicUtility/include/StringAttribute.h>

namespace Topologic
{
	Attribute^ StringAttributeFactory::Create(String ^ key, Object ^ value)
	{
		String^ strValue = safe_cast<String^>(value);
		std::string cppStrValue = msclr::interop::marshal_as<std::string>(strValue);
		return gcnew Attribute(std::dynamic_pointer_cast<TopologicUtility::Attribute>(std::make_shared<TopologicUtility::StringAttribute>(cppStrValue)));
	}

	bool StringAttributeFactory::CheckType(Type ^ type)
	{
		System::Type^ stringHandleType = String::typeid;

		return type == stringHandleType;
	}

	bool StringAttributeFactory::CheckType(const std::shared_ptr<TopologicUtility::Attribute>& kpSupportAttribute)
	{
		return std::dynamic_pointer_cast<TopologicUtility::StringAttribute>(kpSupportAttribute) != nullptr;
	}

	Object^ StringAttributeFactory::CreateValue(const std::shared_ptr<TopologicUtility::Attribute>& kpSupportAttribute)
	{
		void* pValue = kpSupportAttribute->Value();
		std::string* pStringValue = static_cast<std::string*>(pValue);
		return gcnew String(pStringValue->c_str());
	}
}