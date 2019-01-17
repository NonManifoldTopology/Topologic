#include "DoubleAttributeFactory.h"
#include "Attribute.h"

#include <TopologicUtilities/include/DoubleAttribute.h>

#include <memory>

namespace Topologic
{
	Attribute^ DoubleAttributeFactory::Create(String ^ key, Object ^ value)
	{
		double doubleValue = safe_cast<double>(value);
		return gcnew Attribute(std::dynamic_pointer_cast<TopologicUtilities::Attribute>(std::make_shared<TopologicUtilities::DoubleAttribute>(doubleValue)));
	}

	bool DoubleAttributeFactory::CheckType(Type ^ type)
	{
		typedef double ^ DoubleHandle;
		System::Type^ doubleHandleType = DoubleHandle::typeid;

		return type == doubleHandleType;
	}

	bool DoubleAttributeFactory::CheckType(const std::shared_ptr<TopologicUtilities::Attribute>& kpSupportAttribute)
	{
		return std::dynamic_pointer_cast<TopologicUtilities::DoubleAttribute>(kpSupportAttribute) != nullptr;
	}

	Object^ DoubleAttributeFactory::CreateValue(const std::shared_ptr<TopologicUtilities::Attribute>& kpSupportAttribute)
	{
		void* pValue = kpSupportAttribute->Value();
		double* pDoubleValue = static_cast<double*>(pValue);
		return *pDoubleValue;
	}
}