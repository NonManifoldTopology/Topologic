#pragma once

using namespace System;
#ifdef TOPOLOGIC_DYNAMO
using namespace Autodesk::DesignScript::Runtime;
#endif

#include <Attribute.h>

namespace TopologicUtilities
{
	class Attribute;
}

namespace Topologic
{
	namespace Attributes
	{
		/// <summary>
		/// A public abstract factory class to create an Attribute. This class should be inherited when new Attribute classes are implemented.
		/// </summary>
		public ref class AttributeFactory abstract
		{
		public:
			/// <summary>
			/// Creates an Attribute from a key and a value.
			/// </summary>
			/// <param name="key">A key</param>
			/// <param name="value">A value</param>
			/// <returns name="Attribute">The created Attribute</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual Attribute^ Create(String^ key, Object^ value) abstract;

			/// <summary>
			/// Checks the type of the value.
			/// </summary>
			/// <param name="type">The type</param>
			/// <returns name="bool">True if the value is of the correct type, otherwise false</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual bool CheckType(Type^ type) abstract;

		public protected:
			virtual bool CheckType(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute) abstract;

			virtual Object^ CreateValue(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute) abstract;

		protected:
			AttributeFactory() {}
		};
	}
}