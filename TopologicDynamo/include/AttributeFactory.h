#pragma once

using namespace System;
using namespace Autodesk::DesignScript::Runtime;

#include <Attribute.h>

namespace Topologic
{
	public ref class AttributeFactory abstract
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Attribute^ Create(String^ key, Object^ value) abstract;

		[IsVisibleInDynamoLibrary(false)]
		virtual bool CheckType(Type^ type) abstract;

	protected:
		AttributeFactory() {}
	};
}