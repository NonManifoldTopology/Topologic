#include "IntAttributeFactory.h"

#include <TopologicUtility/include/IntAttribute.h>

namespace Topologic
{
	Attribute^ IntAttributeFactory::Create(String ^ key, Object ^ value)
	{
		long long int intValue = safe_cast<long long int>(value);
		return gcnew Attribute(std::dynamic_pointer_cast<TopologicUtility::Attribute>(std::make_shared<TopologicUtility::IntAttribute>(intValue)));
	}

	bool IntAttributeFactory::CheckType(Type ^ type)
	{
		typedef long long int ^ IntHandle;
		System::Type^ intHandleType = IntHandle::typeid;

		return type == intHandleType;
	}

	bool IntAttributeFactory::CheckType(const std::shared_ptr<TopologicUtility::Attribute>& kpSupportAttribute)
	{
		return std::dynamic_pointer_cast<TopologicUtility::IntAttribute>(kpSupportAttribute) != nullptr;
	}

	Object^ IntAttributeFactory::CreateValue(const std::shared_ptr<TopologicUtility::Attribute>& kpSupportAttribute)
	{
		void* pValue = kpSupportAttribute->Value();
		long long int* pIntValue = static_cast<long long int*>(pValue);
		return *pIntValue;
	}
}