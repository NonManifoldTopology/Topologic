#include "IntAttributeFactory.h"

#include <TopologicUtilities/include/IntAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ IntAttributeFactory::Create(String ^ key, Object ^ value)
		{
			long long int intValue = safe_cast<long long int>(value);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicUtilities::Attribute>(std::make_shared<TopologicUtilities::IntAttribute>(intValue)));
		}

		bool IntAttributeFactory::CheckType(Type ^ type)
		{
			typedef long long int ^ IntHandle;
			System::Type^ intHandleType = IntHandle::typeid;

			return type == intHandleType;
		}

		bool IntAttributeFactory::CheckType(const std::shared_ptr<TopologicUtilities::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicUtilities::IntAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ IntAttributeFactory::CreateValue(const std::shared_ptr<TopologicUtilities::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			long long int* pIntValue = static_cast<long long int*>(pValue);
			return *pIntValue;
		}
	}
}