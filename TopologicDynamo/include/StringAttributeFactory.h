#pragma once

#include "AttributeFactory.h"

namespace Topologic
{
	ref class StringAttributeFactory : AttributeFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Attribute^ Create(String^ key, Object^ value) override;

		[IsVisibleInDynamoLibrary(false)]
		virtual bool CheckType(Type^ type) override;

	public protected:
		virtual bool CheckType(const std::shared_ptr<TopologicSupport::Attribute>& kpSupportAttribute) override;

		virtual Object^ CreateValue(const std::shared_ptr<TopologicSupport::Attribute>& kpSupportAttribute) override;

	protected:
	};
}