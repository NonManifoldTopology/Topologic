#include "IntAttributeFactory.h"

#include <TopologicSupport/include/IntAttribute.h>

namespace Topologic
{
	Attribute^ IntAttributeFactory::Create(String ^ key, Object ^ value)
	{
		long long int intValue = safe_cast<long long int>(value);
		return gcnew Attribute(std::dynamic_pointer_cast<TopologicSupport::Attribute>(std::make_shared<TopologicSupport::IntAttribute>(intValue)));
	}

	bool IntAttributeFactory::CheckType(Type ^ type)
	{
		typedef long long int ^ IntHandle;
		System::Type^ intHandleType = IntHandle::typeid;

		return type == intHandleType;
	}
}