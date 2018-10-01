#include "DoubleAttributeFactory.h"
#include "Attribute.h"

#include <TopologicSupport/include/DoubleAttribute.h>

namespace Topologic
{
	Attribute^ DoubleAttributeFactory::Create(String ^ key, Object ^ value)
	{
		double doubleValue = safe_cast<double>(value);
		return gcnew Attribute(std::dynamic_pointer_cast<TopologicSupport::Attribute>(std::make_shared<TopologicSupport::DoubleAttribute>(doubleValue)));
	}

	bool DoubleAttributeFactory::CheckType(Type ^ type)
	{
		typedef double ^ DoubleHandle;
		System::Type^ doubleHandleType = DoubleHandle::typeid;

		return type == doubleHandleType;
	}
}