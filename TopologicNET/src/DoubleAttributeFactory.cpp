#include "DoubleAttributeFactory.h"
#include "Attribute.h"

#include <TopologicCore/include/DoubleAttribute.h>

#include <memory>

namespace Topologic
{
	namespace Attributes
	{
		Attribute^ DoubleAttributeFactory::Create(String ^ key, Object ^ value)
		{
			double doubleValue = safe_cast<double>(value);
			return gcnew Attribute(std::dynamic_pointer_cast<TopologicCore::Attribute>(std::make_shared<TopologicCore::DoubleAttribute>(doubleValue)));
		}

		bool DoubleAttributeFactory::CheckType(Type ^ type)
		{
			typedef double ^ DoubleHandle;
			System::Type^ doubleHandleType = DoubleHandle::typeid;

			return type == doubleHandleType;
		}

		bool DoubleAttributeFactory::CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			return std::dynamic_pointer_cast<TopologicCore::DoubleAttribute>(kpUtilitiesAttribute) != nullptr;
		}

		Object^ DoubleAttributeFactory::CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute)
		{
			void* pValue = kpUtilitiesAttribute->Value();
			double* pDoubleValue = static_cast<double*>(pValue);
			return *pDoubleValue;
		}
	}
}