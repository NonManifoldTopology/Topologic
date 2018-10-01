#include <msclr/marshal_cppstd.h>
#include "StringAttributeFactory.h"

#include <TopologicSupport/include/StringAttribute.h>

namespace Topologic
{
	Attribute^ StringAttributeFactory::Create(String ^ key, Object ^ value)
	{
		String^ strValue = safe_cast<String^>(value);
		std::string cppStrValue = msclr::interop::marshal_as<std::string>(strValue);
		return gcnew Attribute(std::dynamic_pointer_cast<TopologicSupport::Attribute>(std::make_shared<TopologicSupport::StringAttribute>(cppStrValue)));
	}

	bool StringAttributeFactory::CheckType(Type ^ type)
	{
		System::Type^ stringHandleType = String::typeid;

		return type == stringHandleType;
	}

	bool StringAttributeFactory::CheckType(const std::shared_ptr<TopologicSupport::Attribute>& kpSupportAttribute)
	{
		return std::dynamic_pointer_cast<TopologicSupport::StringAttribute>(kpSupportAttribute) != nullptr;
	}

	Object^ StringAttributeFactory::CreateValue(const std::shared_ptr<TopologicSupport::Attribute>& kpSupportAttribute)
	{
		void* pValue = kpSupportAttribute->Value();
		std::string* pStringValue = static_cast<std::string*>(pValue);
		return gcnew String(pStringValue->c_str());
	}
}