#pragma once

#include "AttributeFactory.h"

namespace Topologic
{
	ref class IntAttributeFactory : AttributeFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Attribute^ Create(String^ key, Object^ value) override;

		[IsVisibleInDynamoLibrary(false)]
		virtual bool CheckType(Type^ type) override;
	protected:
	};
}