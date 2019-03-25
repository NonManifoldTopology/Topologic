#include "IntAttributeFactory.h"

#include <TopologicCore/include/IntAttribute.h>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ IntAttributeFactory::Create(String ^ key, Object ^ value)
		{
			long long int intValue = safe_cast<long long int>(value);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicCore::Attribute>(std::make_shared<TopologicCore::IntAttribute>(intValue)));
		}

		bool IntAttributeFactory::CheckType(Type ^ type)
		{
			typedef long long int ^ IntHandle;
			System::Type^ intHandleType = IntHandle::typeid;

			return type == intHandleType;
		}

		bool IntAttributeFactory::CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicCore::IntAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ IntAttributeFactory::CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			long long int* pIntValue = static_cast<long long int*>(pValue);
			return *pIntValue;
		}
	}
}