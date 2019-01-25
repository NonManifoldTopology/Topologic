#pragma once

#include "AttributeFactory.h"

namespace Topologic
{
	/// <summary>
	/// (private) A private factory class to create a double Attribute.
	/// </summary>
	ref class DoubleAttributeFactory : AttributeFactory
	{
	public:
		/// <summary>
		/// Creates an Attribute from a key and a value.
		/// </summary>
		/// <param name="key">A key</param>
		/// <param name="value">A value</param>
		/// <returns name="Attribute">The created Attribute</returns>
		[IsVisibleInDynamoLibrary(false)]
		virtual Attribute^ Create(String^ key, Object^ value) override;

		/// <summary>
		/// Checks the type of the value.
		/// </summary>
		/// <param name="type">The type</param>
		/// <returns name="bool">True if the value is of the correct type, otherwise false</returns>
		[IsVisibleInDynamoLibrary(false)]
		virtual bool CheckType(Type^ type) override;

	public protected:
		virtual bool CheckType(const std::shared_ptr<TopologicUtilities::Attribute>& kpSupportAttribute) override;

		virtual Object^ CreateValue(const std::shared_ptr<TopologicUtilities::Attribute>& kpSupportAttribute) override;

	protected:
	};
}